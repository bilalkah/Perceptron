//Binary veya bipolar input alan ve binary output veren
//63*7 mimarisinde yapay sinir aglari uygulamasi
//3 farkli fontta 7 farkli harfi input olarak alir ve supervised olarak bunlari ogrenen bir ysa
//agirlik guncelleme kurali olarak Delta kurali ve perceptron (yorum satiri olarak) kurali yer almaktadir 

#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<iomanip>

#define FILENUMBER 21       //Input dosyasi sayisi 3 font*7 harf
#define INPUTNUMBER 63      //Bir input dosyasinin icerisindeki veri sayisi
#define T 7                 //Input dosyasinin içerisindeki her bir verinin 7 output noronuyla olan baglantisi
using namespace std;

int main(){

    fstream myFile,inFile;  //myfile filenames.txt dosyasını okuyacak olan, infile ise input olan harf dosyalarını okuyacak olan file tipi  
    string ch;              //Sırasıyla dosyaları açmak için dosyaların ismini tutan string
    char temp;              //Input dosyalarinin icerisindeki birgülleri almak için temp değer
    int **input;            //infile tarafından okunan değerlerin tutulduğu matris
    int i,j,k,sum,fileChoice,ruleChoice,count=0;        //i,j,k iteratör; sum ağırlık*input değerlerini tutmak için, fileChoice binary veya bipolar input seçmek için,
                                                        //count  başarılı öğrenme sayısı
    float alpha,**weight,*bias,rate=0;      //alpha öğrenme kaysayısı, weight ağırlık matrisi, bias bias, rate başarı oranı
    //alpha öğrenme katsayısı, weight ağırlık matrisi, bias bias, rate en son hesaplanacak olan başarı oranı
    int threshold,epoch,ep;    //threshold eşik değeri, epoch tur sayısı
    int target[T][T]={          {1,0,0,0,0,0,0},    //target matrisi her bir harf için hedeflenen output değerlerini gösterir
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

    
    myFile.open("Filenames.txt",ios::in);   //Dosyaların isminin tutulduğu dosyayı açar

    if(!myFile.is_open()){
        cerr << "Hata" << endl;
        exit(EXIT_FAILURE);
    }else{
        cout << "Filenames.txt is opened." << endl;
        cout << "Press 1 for Binary input\nPress 2 for Bipolar input" << endl;
        cin >> fileChoice;  //Binary input mu yoksa bipolar input mu kullanılacağı seçilir 
        i=0;
        while (!myFile.eof() && i!=21)  //filenames.txt dosyasının sonuna gelene kadar ve 21 tane dosya açılana kadar
        {   
            j=0;         
            getline(myFile,ch);     //her bir satırda dosyaların ismi yazıyor

            if(fileChoice==1){
                inFile.open("./Binary_Karakterler_Tam/"+ch, ios::in );  //eğer fileChoice 1 ise binary karakterlerin olduğu dizine gidip dosyaları aç
            }else if(fileChoice==2){
                inFile.open("./Karakterler_Tam/"+ch, ios::in);          //eğer fileChoice 1 ise bipolar karakterlerin olduğu dizine gidip dosyaları aç
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
                    inFile >> input[i][j];  //dosyanın sonuna gelene kadar her bir int değerini okur ve matrise işler, i=kaçıncı dosya, j=kaçıncı int
                    inFile >> temp;         //aradaki virgülü tutar
                    j++;
                }

                i++;
                inFile.close();     //Okuma işlemi bittikten sonra dosyayı kapat
            }
            ch.erase();             //dosyanın ismini tutan stringi temizle, yanlış isim yazılmaması için
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
    /****************************************************************/
    //Epoch sayısı kadar veya %100 başarıya ulaşana kadar tekrar tekrar inputlar YSA'ya girer ve ağırlıklar tekrar hesaplanır
    //Dosyalar sırasıyla girer ve 63 tane data ilk olarak 1. output nöronuna girer ve inputlar nöron ile arasındaki ağırlıklarla çarpılır ve toplanır. 
    //Ardından toplama bias eklenir ve toplam thresholddan geçirilir.
    //çıkan sonuç target değeri ile kıyaslanır ve eğer farklı değerler gelirse mimaride istenilen algoritma kullanılarak ağırlıklar güncellenir ve count sıfıra eşitlenir.
    //Eğer sonuç target değeri ile aynı gelirse count bir arttırılır.
    //Bu 1, 2, 3, .., 7. nöronlar olarak ilerler
    //bir epoch tamamlanmış olur ve epoch bir azaltılarak döngünün başına dönülür
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
                            weight[j][k]+=alpha*(target[i%7][k])*input[i][j];            //Perceptron öğrenmesi
                        }else{
                            weight[j][k]+=alpha*input[i][j]*(target[i%7][k]-sum);        //Delta öğrenmesi
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

    /****************************************************************/
    //Ağırlıkların son halini ve kullanıcının girdiği değerleri weight_matrix.txt isimli dosyaya kaydediyorum.
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

    //Epoch sayısını tamamladıktan sonra veya %100 başarıya ulaştıktan sonra en güncel ağırlıkları kullanarak inputları tekrardan YSA'ya sokup çıktısını yazdırıyorum.
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
   //çıktıları targetla kıyaslayıp başarı oranını hesaplıyorum
   rate=100*rate/(FILENUMBER*T);
   cout << "Success rate: " << setprecision(4) << rate << endl;
   
    /****************************************************************/

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
