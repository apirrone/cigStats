#include <iostream>
#include <time.h>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct tm tm;

typedef struct data{
  tm date;
  int price_of_pack;
  int packs_per_week;
  std::string currency;
} data;


bool checkDate(tm date){
  int then = mktime(&date);
  time_t now = time (NULL);
  
  return (now-then)>0;
}

data readFile(char* file){

  data d;
  
  if(access(file, F_OK)){// File does not exist  else
    std::cout << "ERROR : file does not exist" << std::endl;
    return d;
  }
  
  std::string line;
  std::ifstream myFile(file);
  
  if(myFile.is_open()){

    getline(myFile, line);
    d.date.tm_year = atoi(line.c_str())-1900;
    
    getline(myFile, line);
    d.date.tm_mon = atoi(line.c_str())-1;
    
    getline(myFile, line);
    d.date.tm_mday = atoi(line.c_str());
    
    getline(myFile, line);
    d.date.tm_hour = atoi(line.c_str());
    
    d.date.tm_min=0;
    d.date.tm_sec=0;

    getline(myFile, line);
    d.price_of_pack = atoi(line.c_str());

    getline(myFile, line);
    d.packs_per_week = atoi(line.c_str());

    getline(myFile, line);
    d.currency = line.c_str();
    
    myFile.close();
    return d;
    
  }
  else{
    std::cout << "ERROR : could not read file" << std::endl;
    return d;
  }
  
}

void writeFile(data d, char* filename){
  
  std::ofstream myFile(filename);
  if(myFile.is_open()){
    
    myFile << d.date.tm_year+1900 << "\n";
    myFile << d.date.tm_mon+1  << "\n";
    myFile << d.date.tm_mday << "\n";
    myFile << d.date.tm_hour << "\n";
    myFile << d.price_of_pack << "\n";
    myFile << d.packs_per_week << "\n";
    myFile << d.currency << "\n";
    
    myFile.close();
    
  }
  else
    std::cout << "ERROR : could not write file" << std::endl;  
}

void setup(char* file){

  time_t time;
  
  data d;
  
  int input;
  bool dateOk = false;

  //TODO check inputs (month between 0 and 12 etc)
  do{
    
    std::cout << "Year ?" << std::endl;
    std::cout << ">";
    scanf("%d", &input);
    d.date.tm_year=input-1900;

    std::cout << "Month ?" << std::endl;
    std::cout << ">";
    scanf("%d", &input);
    d.date.tm_mon=input-1;


    std::cout << "Day ?" << std::endl;
    std::cout << ">";
    scanf("%d", &input);
    d.date.tm_mday=input;

    std::cout << "Hour ?" << std::endl;
    std::cout << ">";
    scanf("%d", &input);
    d.date.tm_hour=input;

    d.date.tm_min=0;
    d.date.tm_sec=0;

    dateOk = checkDate(d.date);
    
    if(!dateOk)
      std::cout << "Invalid date ! The date entered cannot be in the future" << std::endl;
    
  } while(!dateOk);


  std::cout << "Price of pack ?" << std::endl;
  std::cout << ">";
  scanf("%d", &input);
  d.price_of_pack=input;

  std::cout << "Packs per week ?" << std::endl;
  std::cout << ">";
  scanf("%d", &input);
  d.packs_per_week=input;


  std::string strInput = "";
  std::cout << "Currency ?" << std::endl;
  std::cout << ">";
  while(strInput == "")
    getline(std::cin, strInput);
  d.currency = strInput;
  
  writeFile(d, file);

  std::cout << "Setup Done !" << std::endl;
}

std::string dateToString(tm date){

  return ""; //TODO
}

void display(char* file){
  
  data d = readFile(file);
  
  tm date = d.date;
  
  std::cout << std::fixed;
  std::cout << std::setprecision(2);
  
  int start = mktime(&date); // date in seconds elapsed since epoch
  time_t seconds = time (NULL);
  
  float hours = ((seconds - start)/60.)/60.;
  float minutes = (hours - (long)hours)*60;
  
  int daysDisplay = (int)(hours/24);
  int heuresDisplay = (((int)(hours))%24);
  
  int months = (int)(daysDisplay/30); // heuristic
  int remainingDays = daysDisplay%30;

  int years = (int)(months/12);
  int remainingMonths = months%12;

  
  std::cout << "----------------------------------------" << std::endl;
  
  if(years>0){    
    std::cout << years;
    
    if(years > 1)
      std::cout << " years, ";
    else
      std::cout << " year, ";

    if(remainingMonths != 0){
      std::cout << remainingMonths;
      if(remainingMonths>1)
	std::cout << " months, ";
      else
	std::cout << " month, ";
    }
  }
  else{
    if(remainingMonths != 0){
      std::cout << months;    
      if(months>1)
	std::cout << " months, ";
      else
	std::cout << " month, ";
    }
  }

  if(remainingDays!=0){
    std::cout << remainingDays;

    if(remainingDays>1)
      std::cout << " days " ;
    else
      std::cout << " day " ;
  
    std::cout << "and ";
  }
  
  std::cout << heuresDisplay ;

  if(heuresDisplay>1)
    std::cout << " hours ";
  else
    std::cout << " hour ";    
  
  std::cout << "since last cigarette " << dateToString(d.date) << std::endl;

  int hoursInWeek = 24*7;
  int pricePerWeek = d.price_of_pack*d.packs_per_week;
  float pricePerHour = pricePerWeek/(hoursInWeek*1.);
  float saved = hours*pricePerHour;

  std::cout << saved << " " << d.currency << " saved " << std::endl;

  
  std::cout << "----------------------------------------" << std::endl;  
}

void usage(){
  std::cout << "USAGE : ./cigStats" << std::endl;
  std::cout << "OR" << std::endl;
  std::cout << "USAGE : ./cigStats --setup" << std::endl;
}

int main(int argc, char* argv[]){
  
  char* file = (char*)"~/.cigStatsConfig"; // to avoid annoying warning (depreciated conversion from string constant to char*)
  
  if(argc > 2){
    usage();
    return 0;
  }

  if(argc == 2){
    if(strcmp(argv[1], "--setup") !=0){
      usage();
      return 0;
    }
    else
      setup(file);
  }

  display(file);
  
  return 1;
}
