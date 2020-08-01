#ifndef NODE_H
#define NODE_H

#include "sat.h"
#include "Edge.h"


using namespace std;
class Node{
    public:
    // debug id  ,is start  , is end     how many bits should we use to represent distance and color
    Node(int id,int st,int en, int colorBitNum ,int DistanceBitNum,SatSolver& s)
    :solver(s),colorSAT(SATnumber(colorBitNum,s)),distanceSAT(SATnumber(DistanceBitNum,s)),distanceP1(distanceSAT.addbyone()){
        distanceP1.assertequalto(distanceSAT.addbyone());
        startcolor=st;
        endcolor=en;
        nid=id;
    }
    ~Node();
    //match with four edge, assign null if there is boundary ,and also assert one fanin one fanout
    void match(Edge* up,Edge* down,Edge*left,Edge* right);
    // start connects one fanin, end node  connects one fanout
    void assert1In1OutForStartEnd(const vector<int>& isconnect,const vector<int>& isfanout,const vector<bool>& inv);
    // except start and end node, each node connects one fanin and one fanout
    void assert1In1Out(const vector<int>& isconnect,const vector<int>& pointTohigh);
    
    // report which color this node should be
    int reportColor (){return colorSAT.value()+1;}
    string toString(){
        stringstream s1;
        s1<<" scolor "<<startcolor<<" ecolor "<<endcolor<<" id "<<nid<<" colorlen "<<colorSAT.tostring()<<" dis len "<<distanceSAT.tostring()<<"solver: "<<&solver;
        return s1.str();
    }
    SATnumber* getcolor(){return &colorSAT;}
    SATnumber* getdistance(){return  &distanceSAT;}
    SATnumber* getdistanceP1(){return &distanceP1;}
    private: 
    SatSolver& solver;
    // represent the color&distance in solver (binary encode)
    SATnumber colorSAT;
    SATnumber distanceSAT;
    //SATnumber  distanceSAT + 1
    SATnumber distanceP1;
    // matched edge
    vector<Edge*> connectedEdge;
    //if this node is start node than color is startcolor, if not assign -1
    int  startcolor;
    //if this node is end node than color is endcolor, if not assign -1
    int endcolor;
    //debug
    int nid;
    

};
#endif