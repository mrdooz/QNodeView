#include "block_loader.hpp"

unordered_map<string, BlockParam> paramsByName;
vector<BlockParam> params;
unordered_map<string, BlockDef> blockDefs;

//------------------------------------------------------------------------------
BlockDef::Param::Type BlockDef::StringToParamType(const QString& str)
{
  QString tmp = str.toLower();
  if (tmp == "bool")
    return Param::Bool;

  if (tmp == "int")
    return Param::Int;

  if (tmp == "float")
    return Param::Float;

  if (tmp == "float2")
    return Param::Float2;

  if (tmp == "float3")
    return Param::Float3;

  if (tmp == "color")
    return Param::Color;

  return Param::Unknown;
}

//------------------------------------------------------------------------------
string BlockDef::ParamTypeToString(Param::Type type)
{
  switch (type)
  {
  case Param::Bool:
    return "bool";
  case Param::Int:
    return "int";
  case Param::Float:
    return "float";
  case Param::Float2:
    return "float2";
  case Param::Float3:
    return "float3";
  case Param::Color:
    return "color";
  default:
    return "";
  }
}

//------------------------------------------------------------------------------
bool loadBlockDesc(const string& filename)
{
  QFile file(filename.c_str());
  if (!file.open(QIODevice::ReadOnly))
    return false;

  QJsonParseError err;
  QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
  if (err.error != QJsonParseError::NoError)
  {
    qWarning() << err.errorString() << " offset: " << err.offset;
    return false;
  }
  QJsonObject objMain = doc.object();

  // load types
  QJsonArray arrTypes = objMain["types"].toArray();
  if (arrTypes.isEmpty())
  {
    qWarning() << "No types found";
    return false;
  }

  for (const QJsonValue& valType : arrTypes)
  {
    QJsonObject objType = valType.toObject();
    QString typeName = objType["name"].toString();
    BlockParam param{ typeName.toStdString(), objType["id"].toInt() };
    params.push_back(param);
    paramsByName[typeName.toStdString()] = param;
  }

  // load blocks
  QJsonArray arrBlocks = objMain["blocks"].toArray();
  if (arrBlocks.empty())
  {
    qWarning() << "No blocks found";
    return false;
  }

  for (const QJsonValue& valBlock : arrBlocks)
  {
    QJsonObject objBlock = valBlock.toObject();
    QString blockName = objBlock["name"].toString();

    BlockDef blockDef;
    blockDef.name = blockName.toStdString();

    // read inputs
    for (const QJsonValue& valInput : objBlock["inputs"].toArray())
    {
      QJsonObject objInput = valInput.toObject();
      QString inputName = objInput["name"].toString();
      QString type = objInput["type"].toString();

      auto it = paramsByName.find(type.toStdString());
      if (it == paramsByName.end())
      {
        qWarning() << "Unknown parameter type in input: " << type;
        return false;
      }
      blockDef.inputs.push_back(BlockDef::Port{ inputName.toStdString(), it->second });
    }

    // read outputs
    for (const QJsonValue& valOutput : objBlock["outputs"].toArray())
    {
      QJsonObject objOutput = valOutput.toObject();
      QString outputName = objOutput["name"].toString();
      QString type = objOutput["type"].toString();

      auto it = paramsByName.find(type.toStdString());
      if (it == paramsByName.end())
      {
        qWarning() << "Unknown parameter type in output: " << type;
        return false;
      }

      blockDef.outputs.push_back(BlockDef::Port{ outputName.toStdString(), it->second });
    }

    // read params
    for (const QJsonValue& valParam : objBlock["params"].toArray())
    {
      QJsonObject objParam = valParam.toObject();
      QString paramName = objParam["name"].toString();
      QString strType = objParam["type"].toString();

      BlockDef::Param param;
      param.name = objParam["name"].toString().toStdString();
      param.type = BlockDef::StringToParamType(strType);

      if (param.type == BlockDef::Param::Unknown)
      {
        qWarning() << "Unknown parameter type in output: " << strType;
        return false;
      }

      // just allow int and float default/min/max values

      if (objParam.contains("default_value"))
      {
        if (param.type == BlockDef::Param::Int)
          param.defaultValue = QVariant(objParam["default_value"].toInt());
        else
          param.defaultValue = QVariant((float)objParam["default_value"].toDouble());
      }

      if (objParam.contains("min_value"))
      {
        if (param.type == BlockDef::Param::Int)
          param.minValue = QVariant(objParam["min_value"].toInt());
        else
          param.minValue = QVariant((float)objParam["min_value"].toDouble());
      }

      if (objParam.contains("max_value"))
      {
        if (param.type == BlockDef::Param::Int)
          param.maxValue = QVariant(objParam["max_value"].toInt());
        else
          param.maxValue = QVariant((float)objParam["max_value"].toDouble());
      }

      blockDef.params.push_back(param);
    }

    blockDefs[blockDef.name] = blockDef;
  }

  return true;
}


//------------------------------------------------------------------------------
QJsonArray BlockDefToJson(const BlockDef& blockDef)
{
  QJsonArray arr;
  return arr;
}

//------------------------------------------------------------------------------
BlockDef BlockDefFromJson(const QJsonArray& arrBlockDef)
{
  BlockDef blockDef;
  return blockDef;
}
