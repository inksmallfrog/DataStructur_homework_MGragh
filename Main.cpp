#include <iostream>
#include <fstream>
#include <cstdlib>
#include "MGragh.hpp"

void Print(int &i){
  std::cout << i << " ";
}

const bool ReadFromFile(const char* fileName, MGragh<int> &gragh){
  std::ifstream fileIn(fileName);
  if(!fileIn.is_open()){
    std::cout << "Can't open file " << fileName << std::endl;
    return false;
  }
  int maxNumber;
  int path;
  char buffer[1024];
  fileIn.getline(buffer, 1024);
  maxNumber = atoi(buffer);
  fileIn.getline(buffer, 1024);
  path = atoi(buffer);
  gragh.Clear();
  gragh.ResetSize(maxNumber);
  for(int i = 0; i < path; ++i){
    int beginNode;
    int endNode;
    fileIn >> beginNode;
    fileIn >> endNode;
    fileIn.getline(buffer, 1024);
    gragh.AddNodeAndEdge(beginNode, endNode);
  }
  
  return true;
}

int main(){
  MGragh<int> intGragh(10);

  ReadFromFile("flight.txt", intGragh);
  intGragh.PrintNearest(2, 11);
  return 0;
  /*for(int i = 0; i < 10; ++i){
    intGragh.AddNode(i);
  }
  intGragh.AddEdge(0, 1);
  intGragh.AddEdge(0, 2);
  intGragh.AddEdge(0, 3);
  intGragh.AddEdge(0, 5);
  intGragh.AddEdge(0, 6);
  intGragh.AddEdge(1, 2);
  intGragh.AddEdge(1, 3);
  intGragh.AddEdge(3, 9);
  intGragh.AddEdge(3, 4);
  intGragh.AddEdge(4, 7);
  //  intGragh.AddEdge(7, 8);
  std::cout << intGragh.GetCurrentNodeCount() << std::endl;
  std::cout << intGragh.GetEdgeCount() << std::endl;
  std::cout << intGragh.GetMaxNodeCount() << std::endl;
  intGragh.PrintNearest(0, 7);
  //  intGragh.DeepOrder(Print);
  intGragh.BroadOrder(Print);*/
}
