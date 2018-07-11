#include <iostream>
#include <time.h>
#include <iomanip>

#define CURRENCY "€"
#define PRICE_OF_PACK 11
#define PACKS_PER_WEEK 1


time_t getDateInSeconds(){
  
}

int main(){

  std::cout << std::fixed;
  std::cout << std::setprecision(2);

  time_t start = 1527105520;
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

    
    std::cout << remainingMonths;
    if(remainingMonths>1)
      std::cout << " months, ";
    else
      std::cout << " month, ";    
  }
  else{
    std::cout << months;    
    if(months>1)
      std::cout << " months, ";
    else
      std::cout << " month, "; 
  }
  std::cout << remainingDays;

  if(remainingDays>1)
    std::cout << " days " ;
  else
    std::cout << " day " ;
  
  std::cout << "and ";
  
  std::cout << heuresDisplay ;

  if(heuresDisplay>1)
    std::cout << " hours ";
  else
    std::cout << " hour ";
    
  
  std::cout << "since last cigarette (2018 may 23 at 10PM)" << std::endl;

  int hoursInWeek = 24*7;
  int pricePerWeek = PRICE_OF_PACK*PACKS_PER_WEEK;
  float pricePerHour = pricePerWeek/(hoursInWeek*1.);
  float saved = hours*pricePerHour;

  std::cout << saved << CURRENCY << " saved " << std::endl;

  
  std::cout << "----------------------------------------" << std::endl;
  
  return 1;
}
