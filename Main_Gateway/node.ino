#include <vector>

struct NodeType {
  int dev_code;
  char dev_name[30];
  uint32_t last_update_tick;
  int state[2];
  int current[2];
  int control[2];
};

std::vector<struct NodeType> nodes;

String nodeHandleFromString(String message)
{
  char *message_str = new char[message.length()+1];
  message.toCharArray(message_str, message.length()+1);
  // dev_code dev_special_name status
  int dev_code, state[2], current[2];
  char dev_name[30];

  int x = 0;
  while(message_str[x] != 0 && x < message.length()+1)
  {
    Serial.print(message_str[x], HEX);
    Serial.print(" ");
    x++;
  }
  Serial.print("\n");
  if (message_str[0] != 0) {
    sscanf(message_str, "%d %s %d %d %d %d", &dev_code, dev_name, &state[0], &state[1], &current[0], &current[1]);
  }
  
  int elem = nodeIsExisting(dev_code, dev_name);
  if (elem >= 0)
  {
    // element exists
    nodes[elem].last_update_tick = millis();
    nodes[elem].state[0] = state[0];
    nodes[elem].state[1] = state[1];
    nodes[elem].current[0] = current[0];
    nodes[elem].current[1] = current[1];

    Serial.println("node existed");
  }
  else
  {
    // add element
    struct NodeType new_node;
    new_node.dev_code = dev_code;
    new_node.last_update_tick = millis();
    strcpy(new_node.dev_name, dev_name);
    new_node.state[0] = state[0];
    new_node.state[1] = state[1];
    new_node.current[0] = current[0];
    new_node.current[1] = current[1];
    new_node.control[0] = state[0];
    new_node.control[1] = state[1];
    nodes.push_back(new_node);
    elem = nodes.size()-1;
    Serial.println("add new node");
  }

  delete message_str;

  String retval;
  retval.reserve(100);
  retval += String(nodes[elem].dev_code) + " ";
  retval += nodes[elem].dev_name;
  retval += " ";
  retval += String(nodes[elem].control[0]);
  retval += " ";
  retval += String(nodes[elem].control[1]) + "\r";
  return retval;
}

// returns element number if existing
// return -1 if element not existing
int nodeIsExisting(int dev_code, char dev_name[30])
{
  int retval = -1;
  for(int i = 0; i < nodes.size(); i++)
  {
    if (nodes[i].dev_code == dev_code && strcmp(nodes[i].dev_name, dev_name) == 0) { retval = i; break; }
  }

  return retval;
}

int nodeSwitchAll(int newstate)
{
  if (newstate != 0 && newstate != 1) newstate = 0;
  if (nodes.size() > 0)
  {
    for(int i=0; i < nodes.size(); i++)
    {
      nodes[i].control[0] = newstate;
      nodes[i].control[1] = newstate;
    }
  }
}
