#include<iostream>
#include<math.h>
using namespace std;
#define ll long long 
ll pow40=pow(2,40);  // or do (1<<40)
ll pow20=pow(2,20);  // or do (1<<20)
ll pow32=pow(2,32);  // or do (1<<32)
ll pow12=pow(2,12);  // or do (1<<12)


void reg_values(ll ir,ll mar,ll mbr,ll ibr,ll ac,ll mq){
    cout<<"IR = "<<ir<<endl;
    cout<<"MAR = "<<mar<<endl;
    cout<<"IBR = "<<ibr<<endl;
    cout<<"MBR = "<<mbr<<endl;
    cout<<"AC = "<<ac<<endl;
    cout<<"MQ = "<<mq<<endl;
}

void decode_instruction(ll ir,ll mar){
    switch(ir){
        case 0: printf("NOP\n");break;
        case 1: printf("ADD M(%lld)\n",mar);break;
        case 2: printf("SUB M(%lld)\n",mar);break;    //data transfer
        case 3: printf("LOAD M(%lld)\n",mar);break;
        case 4: printf("STOR M(%lld)\n",mar);break;
        case 5: printf("JUMP+ M(%lld,0:19)\n",mar);break;     //Conditional Branch
        case 6: printf("JUMP+ M(%lld,20:39)\n",mar);break;
        case 7: printf("LOAD MQ\n");break;
        case 8: printf("LOAD MQ,M(%lld)\n",mar);break;
        case 9: printf("MUL M(%lld)\n",mar);break;
        case 10: printf("DIV M(%lld)\n",mar);break;
        case 11: printf("LSH");break;
        case 12: printf("RSH");break;
        case 13: printf("STOR M(%lld,28:39)\n",mar);break;
        case 14: printf("STOR M(%lld,8:19)\n",mar);break;
        case 15: printf("JUMP M(%lld,0:19)\n",mar);break;
        case 16: printf("JUMP M(%lld,20:39)\n",mar);break;
        case 255: printf("HALT\n");break;        
    }
}

int main(){
    ll memory[200];  //Starting 100 space reserved for instructions and ending 100 space reserved for data

    //Program 1
    memory[10]=0b0000001100000110010000000010000001100101;  //LOAD M(100) SUB M(101)
    memory[11]=0b0000000000000000000000000101000000001101;  // NOP     JUMP+ M(13,0:19)
    memory[12]=0b0000001100000110010100000010000001100100;  //LOAD M(101) SUB M(100)
    memory[13]=0b0000010000000110011111111111000000000000;  //STOR M(103)  HALT
    memory[100]=10;
    memory[101]=10; //memory[103]-reversed

    //Program 2
    memory[20]=0b0000001100000111100000000010000001111001;  //LOAD M(120) SUB M(121)
    memory[21]=0b0000000000000000000000000101000000011000;  // NOP    JUMP+ M(24,0:19)
    memory[22]=0b0000000100000111100100001011000000000000;  // ADD M(121) LSH
    memory[24]=0b0000010000000111101011111111000000000000;  // STOR M(122) HALT
    memory[120]=10;
    memory[121]=5; //memory[123]-reversed
    
    int n; ll pc;
    cout<<"Enter the program number(1 or 2) which you want to implement: ";
    cin>>n;
    if(n==1)
         pc=10; 
    else if(n==2)
         pc=20;
    else
    {cout<<"Please Enter correct program number"<<endl; exit(1);}

    int execute=1;
    ll ir=0;
    ll mar=0;
    ll ibr=0;
    ll mbr=0;
    ll ac=0;
    ll count=0;
    ll mq=0;
    while(execute){
        count++;
        cout<<"PC="<<pc<<endl;
        cout<<"Fetching Instruction"<<endl;
        if(ibr==0){
            mar=pc;
            mbr=memory[mar];
            if((mbr/pow20 == 0)){
                ir=(mbr%pow20)/pow12;
                mar=mbr%pow12;
                pc++;
            }
            else{
                ir= mbr/pow32;
                ibr= mbr%pow20;
                mar= (mbr/pow20)%pow12;
            }}
        else{
            ir=ibr/pow12;
            mar=ibr%pow12;
            ibr=0;
            pc++;
        }
        cout<<"Decimal Values of Registers after Fetch Stage:"<<endl;
        reg_values(ir,mar,mbr,ibr,ac,mq);
        
        cout<<endl<<"Decoding Instruction"<<endl;
        decode_instruction(ir,mar);

        cout<<endl<<"Executing Instruction"<<endl;
        switch(ir){
            case 0: printf("NOP");
                    break;

            case 1: mbr=memory[mar];
                    ac+=mbr;
                    break;  

            case 2: mbr=memory[mar];
                    ac-=mbr;
                    break;

            case 3: mbr=memory[mar];
                    ac=mbr;
                    break;

            case 4: mbr=ac;
                    memory[mar]=mbr;
                    break;

            case 5: if(ac>0){
                        pc=mar;
                        ibr=0;}
                    break;

            case 6: if(ac>0){
                        pc=mar;
                        mbr=memory[mar];
                        ibr=mbr%pow20;}
                    break;

            case 7: ac=mq; 
                    break;        

            case 8: mbr=memory[mar];
                    mq=mbr;
                    break;

            case 9: mbr=memory[mar];
                    ac=(memory[mar]*mq)/pow40;
                    mq=(memory[mar]*mq)%pow40;
                    break;

            case 10:mbr=memory[mar];
                    mq=ac/mbr;
                    ac=ac%mbr;
                    break;

            case 11:ac=ac*2;
                    break;

            case 12:ac=ac/2;
                    break;

            case 13:memory[mar]/=pow12;
                    memory[mar]*=pow12;        
                    memory[mar]+=ac%pow12;
                    break;

            case 14:ll dummy;
                    dummy=memory[mar]%pow32;
                    memory[mar]/=pow32;
                    memory[mar]*=pow12;
                    memory[mar]+=ac%pow12;
                    memory[mar]+=dummy%pow20;
                    break;

            case 15:pc=mar;
                    ibr=0;
                    break;

            case 16:pc=mar;
                    mbr=memory[mar];
                    ibr=mbr%pow20;
                    break;        

            case 255: cout<<"HALTING PROGRAM!"<<endl;
                      execute=0;
                      break;        
        }
        cout<<"Decimal Values of Registers after Execute Stage:"<<endl;
        reg_values(ir,mar,mbr,ibr,ac,mq);
        
        cout<<"--------------------------------End of Cycle "<<count<<" --------------------------------"<<endl;
    }
    cout<<"PROGRAM HALTED SUCCESSFULLY"<<endl;
}


/*
PROGRAM-1
    int a=5;
    int b=10;
    if(a>b)
        c=a-b;
    else
        c=b-a;
*/
/*
PROGRAM-2
    int a=5;
    int b=10;
    if(a>b)
        c=a/b;     //
    else
        c=a*2;
*/        