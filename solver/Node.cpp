#include "Node.h"
#include <vector>
using namespace std;

void Node::match(Edge *up, Edge *down, Edge *left, Edge *right)
{
  vector<int> isconnected;
  vector<int> isfanout;
  vector<bool> invdirection;
  vector<int> connectedAndisfanout;
  vector<int> connectedAndisfanin;
  bool inv=false;
  if(endcolor != -1)inv=true;
  if (startcolor != -1 || endcolor != -1)
  {
    if (up != nullptr)
    {
      isconnected.push_back(up->getconnectid());
      isfanout.push_back(up->getdirectionid());
      invdirection.push_back(inv);
    }
    if (right != nullptr)
    {
      isconnected.push_back(right->getconnectid());
     isfanout.push_back(right->getdirectionid());
      invdirection.push_back(!inv);
    }
    if (down != nullptr)
    {
      isconnected.push_back(down->getconnectid());
     isfanout.push_back(down->getdirectionid());
      invdirection.push_back(!inv);
    }
    if (left != nullptr)
    {
      isconnected.push_back(left->getconnectid());
     isfanout.push_back(left->getdirectionid());
      invdirection.push_back(inv);
    }
  }else{
      if (up != nullptr)
    {
      connectedAndisfanout.push_back(up->getconnectAndPoint2H());
      connectedAndisfanin.push_back(up->getconnectAndPoint2L());
    }
    if (right != nullptr)
    {
      connectedAndisfanout.push_back(right->getconnectAndPoint2L());
      connectedAndisfanin.push_back(right->getconnectAndPoint2H());
    }
    if (down != nullptr)
    {
      connectedAndisfanout.push_back(down->getconnectAndPoint2L());
      connectedAndisfanin.push_back(down->getconnectAndPoint2H());
    }
    if (left != nullptr)
    {
      connectedAndisfanout.push_back(left->getconnectAndPoint2H());
      connectedAndisfanin.push_back(left->getconnectAndPoint2L());
    }
  }
  if (startcolor != -1)
  {
    colorSAT.assertisN(startcolor);
    distanceSAT.assertisN(0);
    assert1In1OutForStartEnd(isconnected,isfanout,invdirection);

  }
  else if (endcolor != -1)
  {
    colorSAT.assertisN(endcolor);
    assert1In1OutForStartEnd(isconnected,isfanout,invdirection);
  }else{
    assert1In1Out(connectedAndisfanout,connectedAndisfanin);
  }
}
void Node::assert1In1OutForStartEnd(const vector<int>& isconnect,const vector<int>& isfanout,const vector<bool>& inv)
{
  
  int constTrue;

  solver.AlwaysOnlyOneTrue(isconnect);
  
  for(int i=0;i<isfanout.size();i++){
    constTrue=solver.newVar();
    
    solver.addImpliedCNF(constTrue,isconnect[i],false,isfanout[i],inv[i]);
    solver.assertProperty(constTrue,true);
  }
  
}
void Node::assert1In1Out(const vector<int>& connectedAndisfanout,const vector<int>& connectedAndisfanin){
  
  
  solver.AlwaysOnlyOneTrue(connectedAndisfanin);
  solver.AlwaysOnlyOneTrue(connectedAndisfanout);
  
}

Node::~Node()
{
}