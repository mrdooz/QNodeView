#pragma once
#include "precompiled.hpp"

struct BlockParam
{
  string name;
  int id;
};

struct BlockDef
{
  string name;
  string type;

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

extern unordered_map<string, BlockParam> PARAMS_BY_NAME;
extern vector<BlockParam> PARAMS;
extern unordered_map<string, BlockDef> BLOCK_DEFS;
extern int NEXT_BLOCK_ID;

extern QJsonArray BlockDefParamsToJson(const vector<BlockDef::Param>& PARAMS);
extern vector<BlockDef::Param> BlockDefParamsFromJson(const QJsonArray& arrBlockDef);

struct Float2
{
  float x, y;
};

struct Float3
{
  float x, y, z;
};

Q_DECLARE_METATYPE(Float2);
Q_DECLARE_METATYPE(Float3);
