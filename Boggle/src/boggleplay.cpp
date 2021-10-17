// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "bogglegui.h"
void popUpInterface(string& humanWord, Vector<string>& humanWords, Boggle& game);
void playOneGame(Lexicon& dictionary) {
    string rand;
    while(true){
        rand = getLine("Do you want to generate a random board?(y/n) ");
        if(rand == "y" || rand =="n") break;
    }
    string boardText;
    if(rand == "n"){
        boardText = getLine("Give me your board (16 letters: ");
    }
    else boardText = "";

    //Human turn
    cout<<"it's your turn!"<<endl;
    Boggle myGame(dictionary, boardText);
    cout<<myGame<<endl;
    Vector<string> humanWords;
    string humanWord;

    popUpInterface(humanWord, humanWords, myGame);
    cout<<"..."<<endl;
    cout<<"Your words ("<<humanWords.size()<<"): ";
    cout<<humanWords<<endl;
    cout<<"Your score: "<<myGame.getScoreHuman()<<endl;
    while(true){
        int sz = humanWords.size();
        humanWord = getLine("Type a word (or Enter to stop):");
        if(humanWord.empty()) break;
        humanWord = toUpperCase(humanWord);
        bool wordOk= myGame.checkWord(humanWord);
        if(wordOk) wordOk *= myGame.humanWordSearch(humanWord);
        else {
            BoggleGUI::setStatusMessage("Your word does not existe");
        }
        if(wordOk){
            humanWords.add(humanWord);
        }
        if(humanWords.size()> sz){
            cout<<"You found a new word! "<< humanWord<<endl;
            cout<<"..."<<endl;
            cout<<"Your words ("<<humanWords.size()<<"): ";
            cout<<humanWords<<endl;
            cout<<"Your score: "<<myGame.getScoreHuman()<<endl;
       }
    }


    cout<<"it's my turn"<<endl;
    Set<string> computerWords = myGame.computerWordSearch();
    cout<<"My words ("<<computerWords.size()<<"): ";
    cout<<computerWords<<endl;
    for(string w: computerWords){
        BoggleGUI::recordWord(w, BoggleGUI::COMPUTER);
    }
    if(myGame.getScoreHuman() < myGame.getScoreComputer()){
        cout<<"My score: "<<myGame.getScoreComputer()<<" Ha ha ha, I destroyed you. Better luck next time, puny human!"<<endl;
        BoggleGUI::setStatusMessage( "Ha ha ha, I destroyed you. Better luck next time, puny human!");
    }
}

void popUpInterface(string& humanWord, Vector<string>& humanWords, Boggle& game){
    while(true){
        humanWord = getLine();
        if(humanWord.empty()) break;
        humanWord = toUpperCase(humanWord);
        bool wordOk = game.checkWord(humanWord);
        if(wordOk) wordOk *= game.humanWordSearch(humanWord);
        else {
            BoggleGUI::setStatusMessage("Your word does not existe");
        }
        if(wordOk){
            humanWords.add(humanWord);
        }
    }
}
