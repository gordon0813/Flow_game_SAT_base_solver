#ifndef SATNUMBER_H
#define SATNUMBER_H
#include "sat.h"
#include <vector>
#include <sstream>
using namespace std;
class SATnumber{
    public:
    SATnumber(int nbits,SatSolver&s): solver(s){
        assert(nbits>0);
        for(int i=0;i<nbits;i++){
            numberid.push_back(solver.newVar());
            //cout<<" num : "<<numberid[i];
        }
    }
    // assert this number always be 0 ( unused )
    void assertzero();
    // assert this number always be N
    void assertisN(int n);
    // return a SATnumber that always greater than this number by 1
    SATnumber addbyone();
    // two number is equal  <==> return variable(var in solver)
    int assertequalto(SATnumber);
    // value of this number
    int value();
    string tostring(){
        stringstream s1;
        s1<<" bits "<<numberid.size();
        return s1.str();
    }
    private:

    vector<int> numberid;
    SatSolver& solver;
};
#endif