#include "Exception/exception.h"
#include "Trading/trading.h"
#include "Utils/utils.h"
#include "Bots/tradingBots.h"
#include "Bots/smartDCA.hpp"
#include "Math/math.h"
//#include "Math/math.ipp"

using namespace std;

int main(){
    //Test class tab
    /*Tab<int> t1(32);
    for(size_t i = 0; i < t1.size(); i++){
        t1[i] = i;
    }
    cout << [&]()->string{string s; for(size_t i = 0; i < t1.size(); i++){s=s+to_string(t1[i]);}return s;}() << endl;
    Tab<int>::iterator it(t1);
    cout << endl << *it;
    ++it;
    cout << endl << *it;
    cout << endl << (it==t1.begin());
    cout << endl << (it!=t1.end());
    it = Tab<int>::iterator(t1, 31);
    cout << endl << (it==t1.begin());
    cout << endl << (it==t1.end()) << endl;
    for(int x : t1){
        x = 4;
    }
    cout << [&]()->string{string s; for(size_t i = 0; i < t1.size(); i++){s=s+to_string(t1[i]);}return s;}() << endl;
     
    //Test min ,max et search
    Tab<Bougie> t2(4);
    t2[0] = Bougie(1,1,1,1);
    t2[1] = Bougie(1,1,1,1);
    cout << "le min est " << t1.min() << endl;
    cout << "le max est " << t1.max() << endl;
    cout << "recherche :" << t1.search(10).first << " position : " << t1.search(10).second<< endl;
    cout << "recherche :" << t1.search(40).first << " position : " << t1.search(40).second<< endl;*/
     
     //Test de TabExt
     /*tabExt<string> tab(2);
     tab.get(1)="z";
     cout << endl << "Result : " << tab.getConst(0) << endl;
     //Exception::BYPASS = true;
     tab.pushBack("2");
     cout << endl << "Result : " << tab.getCopie(2) << endl;
     tab + "2"+ "e";
     cout << endl << "Result : " << tab.getCopie(4) << endl;
     tab.remove(3);
     cout << endl << "Result : " << tab.getCopie(3) << endl;
     cout << endl << "Taille aloué : " << tab.allocSize() << endl;

     //test de TabExt et de son itérateur
     tabExt<size_t> tabTest;
     for(size_t i =0; i<20;  ++i){
          tabTest.pushBack(i*2);
     }
     for(auto x: tabTest){
          cout << "Element :" << x << endl;
     }*/
    
    //Situation à réglé plus tard :
    /*string s = "3.543521";
	Bougie B("2.5","t;!1",s,"1");
	B.affiche();*/
    
    
	/*Bougie B1(2,2,2,1);
    tab.get(1)=B1;
	Bougie B2(1,2,3,1);
    tab.get(2)=B2;
	Bougie B3(3,2,4,1);
    tab.get(3)=B3;
	Bougie B4(2,2,5,1);
    tab.get(4)=B4;
    tab+B4+B2;
    for(int i=1; i<=tab.Taille(); i++){
        tab.get(i).affiche();}*/
    //TabExt2<int> tab(0,2);
    //tab.get(4,2);

    //Table T("test2");
    //T.affiche();
    //T.CSV("test.txt");
    //cout << readFile("Donnée/test");
    
    /*TimeFrame tf(1,TimeUnit::d);
    TimeFrame tf2;
    tf.affiche();
    cout << tf.duree() << endl;
    cout << tf.duree(tf2) << endl;*/

    /*TimeFrame tf(1,TimeUnit::m);
    Chart C("Donnée/test2", tf);
    C.toFile("Donnée/result");*/

    //cout<< Random::normalT(3000000000).get(1) << endl;
    
     //test math & stat fonc
     /*tab<double> tabTest(10);
     for(size_t i =1; i<10;  ++i){
          tabTest[i] = i*2;
     }
     for(auto x: tabTest){
         cout << "Element :" << x << endl;
     }
     cout << "mean :" << mean<tab<double>>(tabTest) << endl;
     cout << "var :" << var<tab<double>>(tabTest) << endl;
     cout << "sd :" << sd<tab<double>>(tabTest) << endl;
     cout << "mean :" << mean<tab<double>, 20>(tabTest) << endl;
     cout << "var :" << var<tab<double>, 20>(tabTest) << endl;
     cout << "sd:" << sd<tab<double>, 20>(tabTest) << endl;*/

     //test Histogramme
     /*int i = 0;
     Tab<double> tabTest(100);
     for(double & x: tabTest){
          x += ++i;
     }
     Histogram histoTest(tabTest);
     histoTest.print();
     //cout << histoTest.getClasse(83).inf <<endl;
     Tab<double> tabTest2(1000);
     i = 0;
     for(double & x: tabTest2){
          x += ++i/2;
     }
     Histogram histoTest2(tabTest2);
     histoTest2.print();
     cout << "recherche de valeur : " << endl;
     histoTest.getClasse(100).print();
     cout << "Proba 1 : " << histoTest.probaInterval(1,91) << endl;
     cout << "Proba 2 : " << histoTest2.probaInterval(-55,91) << endl;
     cout << "Score 1 : " << Histogram::compair(histoTest, histoTest2) << endl;
     Histogram::switchFonctionCompair(1);
     cout << "Score 2 : " << Histogram::compair(histoTest, histoTest) << endl;
     Histogram::switchFonctionCompair(0);
     Histogram::switchFonctionNbClasse(1);
     Histogram histoTest3(tabTest2);
     histoTest3.print();
     cout << "Score 3 : " << Histogram::compair(histoTest3, histoTest) << endl;
     Histogram::switchFonctionNbClasse(2);
     Histogram histoTest4(tabTest2);
     histoTest4.print();
     cout << "Score 4 : " << Histogram::compair(histoTest4, histoTest) << endl;
    cout << "SUCCES \n";*/
    
     //SmartDCA test
     TimeFrame tf(1,TimeUnit::h);
     SmartDCA dca("./Donnée/empty", tf);
     TrainingEnvironment<SmartDCA> TE("./Donnée/testStocReturnShort", tf, dca);
     TE.launchTraining();
     return 0;
}

