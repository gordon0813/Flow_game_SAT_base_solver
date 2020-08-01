/****************************************************************************
  FileName     [ sat.h ]
  PackageName  [ sat ]
  Synopsis     [ Define miniSat solver interface functions ]
  Author       [ Chung-Yang (Ric) Huang, Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2010-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SAT_H
#define SAT_H

#include <cassert>
#include <iostream>
#include <vector>
#include "Solver.h"

using namespace std;

/********** MiniSAT_Solver **********/
class SatSolver
{
   public : 
      SatSolver():_solver(0) { }
      ~SatSolver() { }

      // Solver initialization and reset
      void initialize() {
         reset();
         if (_curVar == 0) { _solver->newVar(); ++_curVar; }
      }
      void reset() {
         if (_solver) delete _solver;
         _solver = new Solver();
         _assump.clear(); _curVar = 0;
      }

      // Constructing proof model
      // Return the Var ID of the new Var
      inline Var newVar() { _solver->newVar(); return _curVar++; }
      // fa/fb = true if it is inverted
      void addAigCNF(Var vf, Var va, bool fa, Var vb, bool fb) {
         vec<Lit> lits;
         Lit lf = Lit(vf);
         Lit la = fa? ~Lit(va): Lit(va);
         Lit lb = fb? ~Lit(vb): Lit(vb);
         lits.push(la); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
         lits.push(lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
         lits.push(~la); lits.push(~lb); lits.push(lf);
         _solver->addClause(lits); lits.clear();
      }
      // fa/fb = true if it is inverted
      void addXorCNF(Var vf, Var va, bool fa, Var vb, bool fb) {
         vec<Lit> lits;
         Lit lf = Lit(vf);
         Lit la = fa? ~Lit(va): Lit(va);
         Lit lb = fb? ~Lit(vb): Lit(vb);
         lits.push(~la); lits.push( lb); lits.push( lf);
         _solver->addClause(lits); lits.clear();
         lits.push( la); lits.push(~lb); lits.push( lf);
         _solver->addClause(lits); lits.clear();
         lits.push( la); lits.push( lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
         lits.push(~la); lits.push(~lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
      }
      //(¬A ∨ ¬B ∨ vf) ∧ (A ∨ B ∨ vf) ∧ (¬B ∨ A ∨ ¬vf) ∧ (¬A ∨ B ∨ ¬vf)
      void addEqualCNF(Var vf, Var va, bool fa, Var vb, bool fb){
         vec<Lit> lits;
         Lit lf = Lit(vf);
         Lit la = fa? ~Lit(va): Lit(va);
         Lit lb = fb? ~Lit(vb): Lit(vb);
         lits.push(~la); lits.push( ~lb); lits.push( lf);
         _solver->addClause(lits); lits.clear();
         lits.push(la); lits.push( lb); lits.push( lf);
         _solver->addClause(lits); lits.clear();
         lits.push(la); lits.push( ~lb); lits.push( ~lf);
         _solver->addClause(lits); lits.clear();
         lits.push(~la); lits.push( lb); lits.push( ~lf);
         _solver->addClause(lits); lits.clear();
      }
      //(¬A ∨ C) ∧ (¬B ∨ C) ∧ (¬C ∨ A ∨ B)
      void addORCNF(Var vf, Var va, bool fa, Var vb, bool fb){
         vec<Lit> lits;
         Lit lf = Lit(vf);
         Lit la = fa? ~Lit(va): Lit(va);
         Lit lb = fb? ~Lit(vb): Lit(vb);
         lits.push(~la); lits.push(lf);
         _solver->addClause(lits); lits.clear();
         lits.push(~lb); lits.push(lf);
         _solver->addClause(lits); lits.clear();
         lits.push(la); lits.push(lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
      }
      //   c<==>(a ==> b)  (A ∨ C) ∧ (¬B ∨ C) ∧ (¬C ∨ ¬A ∨ B)
      void addImpliedCNF(Var vf, Var va, bool fa, Var vb, bool fb){
         vec<Lit> lits;
         Lit lf = Lit(vf);
         Lit la = fa? ~Lit(va): Lit(va);
         Lit lb = fb? ~Lit(vb): Lit(vb);
         lits.push(la); lits.push(lf);
         _solver->addClause(lits); lits.clear();
         lits.push(~lb); lits.push(lf);
         _solver->addClause(lits); lits.clear();
         lits.push(~la); lits.push(lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
      }
      void addNotCNF(Var vf,Var va){
         vec<Lit> lits;
         Lit lf = Lit(vf);
         Lit la = Lit(va);
         lits.push(~la); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
         lits.push(la); lits.push(lf);
         _solver->addClause(lits); lits.clear();
      }
      int addmultiAigCNF(const vector<int>& vas,const vector<bool>& invs){
         assert(vas.size()>1);
         int in1;
         int out=newVar();
         addAigCNF(out,vas[0],invs[0],vas[1],invs[1]);
         in1=out;
         for(int i=2;i<vas.size();i++){
           out=newVar();
           addAigCNF(out,in1,false,vas[i],invs[i]);
           in1=out;
         }
         return out;
      }
      // Clause: ( ab'c' +a'bc' + a'b'c )  ==> only one true
      void AlwaysOnlyOneTrue(const vector<int>& its){
         vec<Lit> lits;
         vector<bool> invs;
         int out;
         for(int i=0;i<its.size();i++){invs.push_back(true);}
         for(int i=0;i<its.size();i++){
            invs[i]=false;
            out=addmultiAigCNF(its,invs);
            lits.push(Lit(out));
            invs[i]=true;
         }
         _solver->addClause(lits);
      }

      // For incremental proof, use "assumeSolve()"
      void assumeRelease() { _assump.clear(); }
      void assumeProperty(Var prop, bool val) {
         _assump.push(val? Lit(prop): ~Lit(prop));
      }
      bool assumpSolve() { return _solver->solve(_assump); }

      // For one time proof, use "solve"
      void assertProperty(Var prop, bool val) {
         _solver->addUnit(val? Lit(prop): ~Lit(prop));
      }
      bool solve() { _solver->solve(); return _solver->okay(); }

      // Functions about Reporting
      // Return 1/0/-1; -1 means unknown value
      int getValue(Var v) const {
         return (_solver->modelValue(v)==l_True?1:
                (_solver->modelValue(v)==l_False?0:-1)); }
      void printStats() const { const_cast<Solver*>(_solver)->printStats(); }

   private : 
      Solver           *_solver;    // Pointer to a Minisat solver
      Var               _curVar;    // Variable currently
      vec<Lit>          _assump;    // Assumption List for assumption solve
};

#endif  // SAT_H

