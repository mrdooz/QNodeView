#pragma once

struct BlockParam
{
  string name;
  int id;
};

struct BlockDef
{
  string name;

  struct Port
  {
    string name;
    BlockParam param;
  };

  struct Param
  {
    enum Type
    {
      Unknown,
      Bool,
      Int,
      Float,
      Float2,
      Float3,
      Color,
    };
    string name;
    Type type;
    QVariant value;
    QVariant minValue, maxValue;
    QVariant defaultValue;
  };

  static Param::Type StringToParamType(const QString& str);
  static string ParamTypeToString(Param::Type type);

  vector<Port> inputs;
  vector<Port> outputs;
  vector<Param> params;
};

bool loadBlockDesc(const string& filename);

extern unordered_map<string, BlockParam> paramsByName;
extern vector<BlockParam> params;
extern unordered_map<string, BlockDef> blockDefs;

extern QJsonArray BlockDefToJson(const BlockDef& blockDef);
extern BlockDef BlockDefFromJson(const QJsonArray& arrBlockDef);

struct Float2
{
  float x, y;
};

struct Float3
{
  float x, y, z;
};
