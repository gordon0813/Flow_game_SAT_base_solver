#ifndef BOARD_H
#define  BOARD_H

#include <iostream>
#include <vector>
#include "sat.h"
#include "Node.h"

using namespace std;
class Board{
    public:
    Board(const string& filename);

    bool solve();
    string ans2string();
    void unitTest(){
       int temp;
       cout<<"unit test of board"<<endl;
       int tempup;
       int tempdown;
       int templeft;
       int tempright;
       for(int i=0;i<9;i++){
           connectedEdge(i,tempup,tempdown,templeft,tempright);
           cout<<i<<" "<<tempup<<" "<<tempdown<<" "<<templeft<<" "<<tempright<<endl;
       }
       int temphigh;
       int templow;
       for(int i=0;i<6;i++){
           connectedNodeCol(i,temphigh,templow);
           cout<<i<<" "<<temphigh<<" "<<templow<<endl;
       }
       for(int i=0;i<6;i++){
           connectedNodeRow(i,temphigh,templow);
           cout<<i<<" "<<temphigh<<" "<<templow<<endl;
       }
      // cout<<"next to horizon edge 1"<<connectedNodeCol;
    }
    private:
    int numOFbit(int i){
        int re=0;
        while(i!=0){
            i/=2;
            re++;
        }
        return re;
    }
    // some function to help indexing node and edge
    bool from1Dto2D(int k,int& i,int& j){return false;}
    int from2Dto1DNode(int row,int col){return row*boardsize+col;}
    int from2Dto1DColedge(int row,int col){return row*boardsize+col;}
    int from2Dto1DRowedge(int row,int col){return row*(boardsize-1)+col;}
    // up left is high  
    bool connectedNodeRow(int e,int& nh,int& nl){
        int rowid=e/(boardsize-1);
        int colid=e%(boardsize-1);
        nh=from2Dto1DNode(rowid,colid);
        nl=from2Dto1DNode(rowid,colid+1);
        return true;
    }
    // 
    bool connectedNodeCol(int e,int& nh,int& nl){
        int rowid=e/(boardsize);
        int colid=e%(boardsize);
        nh=from2Dto1DNode(rowid,colid);
        nl=from2Dto1DNode(rowid+1,colid);
        return true;
    }
    bool connectedEdge(int n,int& up,int& down,int& left,int& right){
        int rowid=n/(boardsize);
        int colid=n%(boardsize);
        up=(rowid>0)? from2Dto1DColedge(rowid-1,colid):-1;
        down=(rowid<boardsize-1)? from2Dto1DColedge(rowid,colid):-1;
        left=(colid>0)? from2Dto1DRowedge(rowid,colid-1):-1;
        right=(colid<boardsize-1)?from2Dto1DRowedge(rowid,colid):-1;
        return true;
    }
    bool downID(int k,int& target){
        if(k/boardsize==boardsize-1)return false;
        target=k+boardsize;
        return true;
    }
    bool upID(int k,int& target){
        if(k/boardsize==0)return false;
        target=k-boardsize;
        return true;
    }
    bool rightID(int k,int& target){
        if(k%boardsize==boardsize-1)return false;
        target=k+1;
        return true;
    }
    bool leftID(int k,int& target){
        if(k%boardsize==0)return false;
        target=k-1;
        return true;
    }
    // also building assertion for solver
    bool connectAllEdgeToNode();
    bool connectAllNodeToedge();
    SatSolver solver;
    vector<Node> nodes;
    vector<Edge> edger;
    vector<Edge> edgec;
    int boardsize;
    int colornumber;
    int distanceBits;
    int colorBits;
    bool issat;


};
#endif