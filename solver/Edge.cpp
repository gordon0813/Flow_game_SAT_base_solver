
#include "Edge.h"

using namespace std;

Edge::~Edge(){

}

void Edge::assertSameColor( SATnumber& highcolor,SATnumber& lowcolor){
  assert(!alreadycolor);
  int consttrue=solver.newVar();
  solver.addImpliedCNF(consttrue,connectid,false, highcolor.assertequalto(lowcolor) ,false);
  solver.assertProperty(consttrue,true);
  alreadycolor=true;
}

void Edge::assertDistanceDiffBy1(SATnumber& highdis,SATnumber& highdisP1,SATnumber& lowdis,SATnumber& lowdisP1){
  assert(!alreadydis);
  int consttrue=solver.newVar();
  solver.addImpliedCNF(consttrue,connectAndPoint2H,false,lowdisP1.assertequalto(highdis),false);
  solver.addImpliedCNF(consttrue,connectAndPoint2L,false,highdisP1.assertequalto(lowdis),false);
  solver.assertProperty(consttrue,true);
  alreadydis=true;
}
