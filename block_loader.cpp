#include "block_loader.hpp"

unordered_map<string, BlockParam> paramsByName;
vector<BlockParam> params;
unordered_map<string, BlockDef> blockDefs;

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
      blockDef.inputs.push_back(BlockDef::Node{ inputName.toStdString(), it->second });
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

      blockDef.outputs.push_back(BlockDef::Node{ outputName.toStdString(), it->second });
    }

    blockDefs[blockDef.name] = blockDef;
  }

  return true;
}
