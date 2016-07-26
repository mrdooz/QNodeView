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

  vector<Port> inputs;
  vector<Port> outputs;
  vector<Param> params;
};

bool loadBlockDesc(const string& filename);

extern unordered_map<string, BlockParam> paramsByName;
extern vector<BlockParam> params;
extern unordered_map<string, BlockDef> blockDefs;
