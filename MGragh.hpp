#ifndef _H_MGRAGH_
#define _H_MGRAGH_

#include <iostream>

#ifndef NULL
#define NULL 0
#endif

template <class T>
class MGragh{
 public:
  MGragh(int maxNodeCount);
  ~MGragh();
  const bool IsEmpty()const;
  const int GetMaxNodeCount()const;
  const int GetCurrentNodeCount()const;
  const int GetEdgeCount()const;
  void PrintNearest(const T &nodeBegin, const T &nodeEnd)const;
  void Clear();
  void ResetSize(int maxNodeCount);
  const bool AddNode(const T &node);
  const bool AddNodeAndEdge(const T &nodeBegin, const T &nodeEnd, int weight = 1);
  const bool AddEdge(const T &nodeBegin, const T &nodeEnd, int weight = 1);

  void DeepOrder(void (*Visit)(T &node));
  void BroadOrder(void (*Visit)(T &node));
  
 private:
  const int GetNodeIndex(const T& node)const;
  const T GetIndexNode(const int index)const;

  void DeepOrderNode(void (*Visit)(T &node), int index, bool *visited);
  
  T *m_elements;
  int **m_matrix;
  int m_currentNodeCount;
  int m_maxNodeCount;
};

template <class T>
MGragh<T>::MGragh(int maxNodeCount){
  m_elements = new T[maxNodeCount];
  m_matrix = new int*[maxNodeCount];
  for(int i = 0; i < maxNodeCount; ++i){
    m_matrix[i] = new int[maxNodeCount];
  }
  m_currentNodeCount = 0;
  m_maxNodeCount = maxNodeCount;
}

template <class T>
MGragh<T>::~MGragh<T>(){
  if(m_elements != NULL){
    delete[] m_elements;    
  }
  m_elements = NULL;

  for(int i = 0; i < m_maxNodeCount; ++i){
    delete[] m_matrix[i];
    m_matrix[i] = NULL;
  }

  delete[] m_matrix;
  m_matrix = NULL;

  m_currentNodeCount = 0;
  m_maxNodeCount = 0;
}

template <class T>
const bool MGragh<T>::IsEmpty()const{
  return m_currentNodeCount == 0;
}

template <class T>
const int MGragh<T>::GetMaxNodeCount()const{
  return m_maxNodeCount;
}

template <class T>
const int MGragh<T>::GetCurrentNodeCount()const{
  return m_currentNodeCount;
}

template <class T>
const int MGragh<T>::GetEdgeCount()const{
  int edge = 0;
  for(int from = 0; from < m_currentNodeCount; ++from){
    for(int to = 0; to < m_currentNodeCount; ++to){
      if(m_matrix[from][to] != 0){
        ++edge;
      }
    }
  }
  return edge;
}

template <class T>
void MGragh<T>::PrintNearest(const T &nodeBegin, const T &nodeEnd)const{
  int *queue = new int[m_currentNodeCount];
  int *from = new int[m_currentNodeCount];
  bool *visit = new bool[m_currentNodeCount];
  for(int i = 0; i < m_currentNodeCount; ++i){
    from[i] = -1;
    visit[i] = false;
  }
  int find = -1;
  int queueCurrentPos = 0;
  int queueMaxPos = 0;
  queue[queueMaxPos++] = GetNodeIndex(nodeBegin);
  if(queue[queueCurrentPos] == -1){
    std::cout << "找不到起始节点" << std::endl;
    return;
  }
  visit[queue[queueCurrentPos]] = true;

  for(;queueCurrentPos < queueMaxPos && find == -1; ++queueCurrentPos){
    for(int i = 0; i < m_currentNodeCount; ++i){
      if(m_matrix[queue[queueCurrentPos]][i] && !visit[i]){
        queue[queueMaxPos++] = i;
        from[i] = queue[queueCurrentPos];
        visit[i] = true;
        if(m_elements[i] == nodeEnd){
          find = i;
          break;
        }
      }
    }
  }

  queueMaxPos = 0;
  if(find != -1){
    for(int i = find; from[i] != -1; ++i){
      queue[queueMaxPos++] = from[i];
    }
    std::cout << nodeBegin << ", ";
    for(int i = queueMaxPos - 1; i > -1; --i){
      std::cout << m_elements[queue[i]] << ", ";
    }
    std::cout << nodeEnd << std::endl;
  }
  else{
     std::cout << "找不到目标节点"; 
  }
  return;
}

template <class T>
void MGragh<T>::Clear(){
  m_currentNodeCount = 0;
}

template <class T>
void MGragh<T>::ResetSize(int maxNodeCount){
  T *elements = new T[maxNodeCount];
  for(int i = 0; i < m_currentNodeCount; ++i){
    elements[i] = m_elements[i];
  }
  delete[] m_elements;
  m_elements = elements;
  
  int **matrix = new int*[maxNodeCount];
  for(int i = 0; i < maxNodeCount; ++i){
    matrix[i] = new int[maxNodeCount];
  }
  for(int from = 0; from < m_currentNodeCount; ++from){
    for(int to = 0; to < m_currentNodeCount; ++to){
      matrix[from][to] = m_matrix[from][to];
    }
    delete[] m_matrix[from];
  }
  delete[] m_matrix;
  m_matrix = matrix;
  
  m_maxNodeCount = maxNodeCount;
}

template <class T>
const bool MGragh<T>::AddNode(const T &node){
  for(int i = 0; i < m_currentNodeCount; ++i){
    if(node == m_elements[i]){
      return false;
    }
  }

  if(m_currentNodeCount + 1 > m_maxNodeCount){
    std::cout << "数组越界" << std::endl;
    return false;
  }

  m_elements[++m_currentNodeCount - 1] = node;
}

template <class T>
const bool MGragh<T>::AddNodeAndEdge(const T &nodeBegin, const T &nodeEnd, int weight){
  AddNode(nodeBegin);
  AddNode(nodeEnd);
  AddEdge(nodeBegin, nodeEnd);

  return true;
}

template <class T>
const bool MGragh<T>::AddEdge(const T &nodeFrom, const T &nodeTo, int weight){
  int fromIndex = GetNodeIndex(nodeFrom);
  int toIndex = GetNodeIndex(nodeTo);
  if(-1 == fromIndex || -1 == toIndex){
    std::cout << "不存在该顶点" << std::endl;
    return false;
  }
  if(m_matrix[fromIndex][toIndex] != 0){
    std::cout << "已存在此条边" << std::endl;
    return false;
  }
  m_matrix[fromIndex][toIndex] = weight;
}

template <class T>
void MGragh<T>::DeepOrder(void (*Visit)(T &)){
  bool *visited = new bool[m_currentNodeCount];
  for(int i = 0; i < m_currentNodeCount; ++i){
    visited[i] = false;
  }

  for(int from = 0; from < m_currentNodeCount; ++from){
    if(visited[from]){
      continue;
    }

    DeepOrderNode(Visit, from, visited);
  }
}

template <class T>
void MGragh<T>::DeepOrderNode(void (*Visit)(T &), int index, bool *visited){
  if(visited[index]){
    return;
  }

  Visit(m_elements[index]);
  visited[index] = true;
    
  for(int to = 0; to < m_currentNodeCount; ++to){
    if(m_matrix[index][to] == 0 || visited[to]){
      continue;
    }
    DeepOrderNode(Visit, to, visited);
  }
}

template <class T>
void MGragh<T>::BroadOrder(void (*Visit)(T &)){
  bool *visited = new bool[m_currentNodeCount];
  for(int i = 0; i < m_currentNodeCount; ++i){
    visited[i] = false;
  }

  int *stock = new int[m_currentNodeCount];
  int stock_currentPos = 0;
  int stock_maxPos = 0;
 
  for(int from = 0; from < m_currentNodeCount; ++from){
    if(visited[from]){
      continue;
    }
    stock[stock_maxPos++] = from;
    
    for(;stock_currentPos < stock_maxPos; ++stock_currentPos){
      if(visited[stock[stock_currentPos]]){
        continue;
      }
      Visit(m_elements[stock[stock_currentPos]]);
      visited[stock[stock_currentPos]] = true;

      for(int to = 0; to < m_currentNodeCount; ++to){
        if(m_matrix[stock[stock_currentPos]][to] == 0 || visited[to]){
          continue;
        }
        stock[stock_maxPos++] = to;
      }
    }
  }
}

template <class T>
const int MGragh<T>::GetNodeIndex(const T &node)const{
  for(int i = 0; i < m_currentNodeCount; ++i){
    if(m_elements[i] == node){
      return i;
    }
  }
  return -1;
}

template <class T>
const T MGragh<T>::GetIndexNode(const int index)const{
  if(index >= 0 && index < m_currentNodeCount){
    return m_elements[index];
  }
  else{
    std::cout << "不存在此节点！" << std::endl;
    T no;
    return no;
  }
}

#endif
