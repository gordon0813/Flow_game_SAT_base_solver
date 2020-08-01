#ifndef unitTestNumber
//#define unitTestNumber
#endif
#ifndef unitTestNode
#define unitTestNode
#endif
#ifndef unitTestSolver
//#define unitTestSolver
#endif

#include "Board.h"
#include <fstream> 
#include <iomanip>
using namespace std;

Board::Board(const string& filename){
    int temp ;
    solver.initialize();
    ifstream inputFile(filename);
    inputFile>>boardsize>>colornumber;
    colorBits=numOFbit(colornumber);
    distanceBits=numOFbit(boardsize*boardsize);
    cout<<"num of color: "<<colornumber<<" ,use "<<colorBits<<" bits for color\n";
    cout<<"board size : "<<boardsize<<" ,use "<<distanceBits<<" bits for max distance\n";
    bool* isused =new bool[colornumber];
    int startcolor;
    int endcolor;
    for(int i=0;i<colornumber;i++)isused[i]=false;
    for(int i=0;i<boardsize;i++){
        for(int j=0;j<boardsize;j++){
            inputFile>>temp;
            temp--;
            startcolor=(temp>=0 &&!isused[temp])? temp: -1;
            endcolor=(temp>=0 && isused[temp])?temp:-1;
            nodes.push_back(Node(from2Dto1DNode(i,j),startcolor,endcolor,colorBits,distanceBits,solver));
            cout<<temp+1<<" ";
            isused[temp]=true;
        }
        cout<<endl;
    }
    cout<<endl;
    
    for(int i=0;i<boardsize*(boardsize-1);i++){
        edger.push_back(Edge(i,solver));
        edgec.push_back(Edge(i,solver));
    }
    connectAllEdgeToNode();
    connectAllNodeToedge();


}
bool Board::connectAllEdgeToNode(){
    int idu,idd,idr,idl;
    for(int i=0;i<nodes.size();i++){
        connectedEdge(i,idu,idd,idl,idr);
        nodes[i].match((idu!=-1) ? &edgec[idu]:nullptr 
        ,(idd!=-1) ? &edgec[idd]:nullptr 
        ,(idl!=-1) ? &edger[idl]:nullptr
        ,(idr!=-1) ? &edger[idr]:nullptr);
    }
    return true;
}
bool Board::connectAllNodeToedge(){
    int idh,idl;
    for(int i=0;i<edger.size();i++){
        //row 
        this->connectedNodeRow(i,idh,idl);
        edger[i].assertDistanceDiffBy1(*(nodes[idh].getdistance()),*nodes[idh].getdistanceP1(),*nodes[idl].getdistance(),*nodes[idl].getdistanceP1());
        edger[i].assertSameColor(*nodes[idh].getcolor(),*nodes[idl].getcolor());
        //col
        this->connectedNodeCol(i,idh,idl);
        edgec[i].assertDistanceDiffBy1(*(nodes[idh].getdistance()),*nodes[idh].getdistanceP1(),*nodes[idl].getdistance(),*nodes[idl].getdistanceP1());
        edgec[i].assertSameColor(*nodes[idh].getcolor(),*nodes[idl].getcolor());
    }
    return true;
}
bool Board::solve(){
    cout<<"start solving... this may take a while\n";
    return solver.solve();
}
string Board::ans2string(){
    stringstream ss;
    for (int i=0;i<boardsize;i++){
        for(int j=0;j<boardsize;j++){
            
            ss<<setw(2)<<nodes[from2Dto1DNode(i,j)].reportColor();
            if(j!=boardsize-1){
                ss<<((edger[from2Dto1DRowedge(i,j)].reportConnect())?"---":"   ");
            }else{ss<<endl;}
        }
        if(i!=boardsize-1){
        for(int j=0;j<boardsize;j++){
            ss<<(edgec[from2Dto1DColedge(i,j)].reportConnect() ? " |":"  ");
            if(j!=boardsize-1)ss<<"   ";
            else ss<<endl;
        }
        }
    }
    return ss.str();
}

int main()
{


    #ifdef unitTestNumber
    SatSolver s;
    s.initialize();
    SATnumber sn1(4,s);
    sn1.assertisN(13);
    cout<<"assert zero "<<s.solve()<<endl;
    cout<<"value: "<<sn1.value()<<endl;
    s.reset();
    SATnumber sn2(4,s);
    SATnumber sn3(4,s);
    s.assertProperty(sn3.assertequalto(sn2),true);
    s.solve();
    cout<<sn2.value()<<sn3.value()<<endl;
    s.reset();
    SATnumber sn4(4,s);
    SATnumber sn0(4,s);
    SATnumber sn7(4,s);
    sn0.assertisN(3); //define sn0 as 3 
    SATnumber sn5=sn4.addbyone();//sn4 =sn5+1
    SATnumber sn6=sn5.addbyone();//sn6=sn5+1
    s.assertProperty(sn4.assertequalto(sn0),true);//sn4=0
    s.assertProperty(sn5.assertequalto(sn7),true);//sn5=sn7
    s.solve();
    cout<<sn4.value()<<" "<<sn5.value()<<" "
    <<sn6.value()<<" "<<sn7.value()<<endl;//what is sn4  sn5 sn6 and sn7
    #endif
    #ifdef unitTestNode

    #endif
    #ifdef unitTestSolver
    SatSolver s;
    s.initialize();
    int out;
    cout<<"aig fanin  in1  in2'  in3  in4'"<<endl;
    vector<int> ins;
    ins.push_back(s.newVar()); ins.push_back(s.newVar()); ins.push_back(s.newVar()); ins.push_back(s.newVar());
    vector<bool> invs;
    invs.push_back(false);invs.push_back(true);invs.push_back(false);invs.push_back(true);
    out=s.addmultiAigCNF(ins,invs);
    s.assertProperty(out,true);
    s.solve();
    cout<< "in1 in2 in3 in4 "<<s.getValue(ins[0])<<s.getValue(ins[1])<<s.getValue(ins[2])<<s.getValue(ins[3])<<endl;
    cout<<"one true  in1  in2=false  in3  in4\n";
    s.reset();
    ins.clear();
    ins.push_back(s.newVar()); ins.push_back(s.newVar()); ins.push_back(s.newVar()); ins.push_back(s.newVar());
    s.assertProperty(ins[1],false);
    s.AlwaysOnlyOneTrue(ins);
    s.solve();
    cout<< "in1 in2 in3 in4 "<<s.getValue(ins[0])<<s.getValue(ins[1])<<s.getValue(ins[2])<<s.getValue(ins[3])<<endl;
    s.reset();
    ins.clear();
    int n1,n2,n3;
    n1=s.newVar();
    n2=s.newVar();
    n3=s.newVar();
    s.addImpliedCNF(n1,n2,false,n3,false);
    s.assertProperty(n3,true);
    s.assertProperty(n2,false);
    s.solve();
    cout<<s.getValue(n1)<<endl;
    #endif
        
  
    Board* b=new Board("./data/board1.txt");
    cout<<((b->solve())? (b->ans2string()):("  unsat   \n")) <<endl;
    delete b;
    b=new Board("./data/board2.txt");
    cout<<((b->solve())? (b->ans2string()):("  unsat   \n")) <<endl;
    delete b;
    b=new Board("./data/board3.txt");
    cout<<((b->solve())? (b->ans2string()):("  unsat   \n")) <<endl;
    delete b;
    b=new Board("./data/board4.txt");
    cout<<((b->solve())? (b->ans2string()):("  unsat   \n")) <<endl;
    delete b;
    b=new Board("./data/boardunsat.txt");
    cout<<((b->solve())? (b->ans2string()):("  unsat   \n")) <<endl;
    delete b;
    b=new Board("./data/board5.txt");
    cout<<((b->solve())? (b->ans2string()):("  unsat   \n")) <<endl;
    delete b;
    b=new Board("./data/board6.txt");
    cout<<((b->solve())? (b->ans2string()):("  unsat   \n")) <<endl;
    delete b;
    
}
