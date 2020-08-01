#ifndef EDGE_H
#define EDGE_H

#include "sat.h"
#include "SATnumber.h"
#include<vector>
using namespace std;

class Edge{
    public:
    Edge(int id ,SatSolver&s):solver(s){
        eid=id;
        connectid=solver.newVar();
        directionid=solver.newVar();
        connectAndPoint2H=solver.newVar();
        connectAndPoint2L=solver.newVar();
        solver.addAigCNF(connectAndPoint2H,connectid,false,directionid,false);
        solver.addAigCNF(connectAndPoint2L,connectid,false,directionid,true);
        alreadycolor=false;
        alreadydis=false;

    }
    ~Edge();

    // assert nodes at two side have same color
    void assertSameColor(SATnumber& highcolor,SATnumber& lowcolor);
    // assert high nodes has distance to start node  that is smaller than low node's by 1
    void assertDistanceDiffBy1(SATnumber& highdis,SATnumber& highdisP1,SATnumber& lowdis,SATnumber& lowdisP1);
    // shoud be connected or not
    bool reportConnect(){return solver.getValue(connectid)==1;}
    int getconnectid(){return connectid;}
    int getdirectionid(){return directionid;}
    int getconnectAndPoint2H(){return connectAndPoint2H;}
    int getconnectAndPoint2L(){return connectAndPoint2L;}
    private:
    SatSolver& solver;
    //index of SAT variable
    int connectid;
    // true when point to
    int directionid;
     // true when connected And Point to High ( up,left is high 
    int connectAndPoint2H;
    // true when connectAndPoint2L( down, right is low
    int connectAndPoint2L; 
    // debug
    int eid;
    bool alreadycolor;
    bool alreadydis; 
    
};
#endif
