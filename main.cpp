#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<iomanip>

#define FILENUMBER 21
#define INPUTNUMBER 63 
#define T 7 
using namespace std;

int main(){

    fstream myFile,inFile;
    string ch;
    char temp;
    int **input;
    int i,j,k,sum,fileChoice,ruleChoice,count=0;
    float alpha,**weight,*bias,rate=0;
    int threshold,epoch,ep;
    int target[T][T]={          {1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0},
                                {0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0},
                                {0,0,0,0,1,0,0},
                                {0,0,0,0,0,1,0},
                                {0,0,0,0,0,0,1}
    };
    
    cout << "Enter the learning rate(between 0-1): ";
    cin >> alpha;
    if(alpha <= 0 || alpha > 1){
        cerr << "Learning rate must be between 0 and 1" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Enter the epoch number: ";
    cin >> epoch;
    ep=epoch;
    cout << "Threshold: " ;
    cin >> threshold;
    if(threshold<=0){
        cerr << "Threshold must be greater than 0" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Press 1 for Perceptron rule\nPress 2 for Delta Rule" << endl;
    cin >> ruleChoice;
    if(ruleChoice !=1 && ruleChoice !=2){
        cerr << "Wrong rule choice.." << endl;
        exit(EXIT_FAILURE);
    }
    bias=new float[T]{0};                                                  

    input=new int*[FILENUMBER];                     
    for(i=0;i<FILENUMBER;i++){
        input[i]=new int[INPUTNUMBER];
    }

    weight=new float*[INPUTNUMBER];                             
    for(i=0;i<INPUTNUMBER;i++){
        weight[i]=new float[T]{0.0};
    }

    
    myFile.open("Filenames.txt",ios::in);

    if(!myFile.is_open()){
        cerr << "Hata" << endl;
        exit(EXIT_FAILURE);
    }else{
        cout << "Filenames.txt is opened." << endl;
        cout << "Press 1 for Binary input\nPress 2 for Bipolar input" << endl;
        cin >> fileChoice;
        i=0;
        while (!myFile.eof() && i!=21)
        {   
            j=0;         
            getline(myFile,ch);

            if(fileChoice==1){
                inFile.open("./Binary_Karakterler_Tam/"+ch, ios::in ); 
            }else if(fileChoice==2){
                inFile.open("./Karakterler_Tam/"+ch, ios::in);
            }else{
                cerr << "Wrong choice" << endl;
                exit(EXIT_FAILURE);
            }

            if(!inFile.is_open()){
                cerr << "Hata - " << ch << endl;
                exit(EXIT_FAILURE);
            }else{
                while (!inFile.eof())
                {
                    inFile >> input[i][j]; 
                    inFile >> temp;        
                    j++;
                }

                i++;
                inFile.close(); 
            }
            ch.erase();   
        }
        if(i==21){
            if(fileChoice==1){
                cout << "All .txt files is opened from Binary_Karakterler_Tam directory" << endl;
            }else{
                cout << "All .txt files is opened from Karakterler_Tam directory" << endl;
            }
        }
        myFile.close();
    }
    while(epoch>0 && count<(FILENUMBER*T)){         

        for(i=0;i<FILENUMBER;i++){  
            
            for(k=0;k<T;k++){
                sum=0;
                
                for(j=0;j<INPUTNUMBER;j++){
                    sum+=input[i][j]*weight[j][k];
                }
                sum+=bias[k];
                
                if(sum>threshold){
                    sum=1;
                }else{
                    sum=0;
                }                                  
                if(sum!=target[i%7][k]){
                        
                    for(j=0;j<INPUTNUMBER;j++){
                        if(ruleChoice==1){
                            weight[j][k]+=alpha*(target[i%7][k])*input[i][j];
                        }else{
                            weight[j][k]+=alpha*input[i][j]*(target[i%7][k]-sum);
                        }   
                    }
                    bias[k]+=alpha*target[i%7][k];
                    count=0;
                }else{
                    count++;
                }             
            }
        }
        epoch--;
    }

    myFile.open("weight_matrix.txt",ios::out);
    myFile << "\tY1\tY2\tY3\tY4\tY5\tY6\tY7" << endl;
    for(i=0;i<INPUTNUMBER;i++){
        myFile << "X" << i+1 ;
        for(j=0;j<T;j++){
            myFile << "\t" << setprecision(2) << weight[i][j] ;
        }
        myFile << endl;
    }
    myFile << "Learning rate: " << alpha << endl;
    myFile << "Epoch number: " << ep << endl;
    myFile << "Threshold: " << threshold << endl;
    if(fileChoice==1){
        myFile << "Input type: Binary" << endl;
    }else{
        myFile << "Input type: Bipolar" << endl;
    }
    if(ruleChoice==1){
        myFile << "Rule: Perceptron" << endl;
    }else{
        myFile << "Rule: Delta" << endl;
    }
    myFile.close();
    for(i=0;i<FILENUMBER;i++){
        for(k=0;k<T;k++){
            sum=0;
            for(j=0;j<INPUTNUMBER;j++){
                sum+=weight[j][k]*input[i][j];
            }
            sum+=bias[k];
            if(sum>threshold){
                sum=1;
            }else{
                sum=0;
            }
            if(sum==target[i%7][k]){
                rate++;
            }
            cout << sum << " ";
        }
        cout << endl;
   }
   rate=100*rate/(FILENUMBER*T);
   cout << "Success rate: " << setprecision(4) << rate << endl;
   
    delete[] bias;

    for(i=0;i<INPUTNUMBER;i++){
        delete[] weight[i];
    }
    delete[] weight;

    for(i=0;i<FILENUMBER;i++){
        delete[] input[i];
    }
    delete[] input;


    return 0;
}
