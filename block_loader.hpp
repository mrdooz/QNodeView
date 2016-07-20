#pragma once

struct BlockParam
{
  string name;
  int id;
};

struct BlockDef
{
  string name;

  struct Node
  {
    string name;
    BlockParam param;
  };

  vector<Node> inputs;
  vector<Node> outputs;
};

bool loadBlockDesc(const string& filename);

extern unordered_map<string, BlockParam> paramsByName;
extern vector<BlockParam> params;
extern unordered_map<string, BlockDef> blockDefs;
