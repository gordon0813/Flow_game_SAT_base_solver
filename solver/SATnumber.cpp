    #include"SATnumber.h"
    using namespace std;
    
    void SATnumber::assertzero(){
        for (int i=0;i<this->numberid.size();i++){
            solver.assertProperty(numberid[i],false);
        }
    }
    void SATnumber::assertisN(int n){
        assert(n>-1); 
        for(int i=numberid.size()-1;i>-1;i--){
            solver.assertProperty(numberid[i],n%2==1);
            n=n/2;
        }
        assert(n==0);
    }

    int SATnumber::assertequalto(SATnumber that){
        int in1=solver.newVar();
        int in2;
        int out;
        solver.addEqualCNF(in1,this->numberid[0],false,that.numberid[0],false);
        for(int i=1;i<numberid.size();i++){
            in2=solver.newVar();
            out=solver.newVar();
            solver.addEqualCNF(in2,this->numberid[i],false,that.numberid[i],false);
            solver.addAigCNF(out,in1,false,in2,false);
            in1=out;
        }
        return out;

    }
    SATnumber SATnumber::addbyone(){
        SATnumber re(numberid.size(),solver);
        int tail=numberid.size()-1;
        int carryin=numberid[tail];
        int carryout;
        solver.addNotCNF(re.numberid[tail],this->numberid[tail]);
        for(int i=tail-1;i>-1;i--){
            solver.addXorCNF(re.numberid[i],carryin,false,this->numberid[i],false);
            if(i!=0){
                carryout=solver.newVar();
                solver.addAigCNF(carryout,carryin,false,this->numberid[i],false);
            }
            carryin=carryout;
        }
        return re;
    }
    int SATnumber::value(){
        int v=0;
        int temp;
        for (int i=0;i<numberid.size();i++){
            v=v*2;
            temp=solver.getValue(numberid[i]);
            if(temp>=0){
                v+=temp;
            }else return -1;
        }
        return v;
    }