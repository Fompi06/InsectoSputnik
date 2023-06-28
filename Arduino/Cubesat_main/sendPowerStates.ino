#define VREF 5.01      // точное напряжение на пине 5V (в данном случае зависит от стабилизатора на плате Arduino)
#define DIV_R1 6800  // точное значение 10 кОм резистора
#define DIV_R2 1100   // точное значение 4.7 кОм резистора
#define DIV_R3 6800  // точное значение 10 кОм резистора
#define DIV_R4 4700   // точное значение 4.7 кОм резистора
#define DIV_R5 4700  // точное значение 10 кОм резистора
#define DIV_R6 6800   // точное значение 4.7 кОм резистора

void sendpowerStates()
{
  
  if(millis() - timerPower > 2000)
  {
    timerPower = millis();
    loraSend("3, ");
    loraSend(((float)analogRead(VBat) * VREF * ((DIV_R1 + DIV_R2) / DIV_R2) / 1024) - 1.2);
    loraSend(", ");
    loraSend(((float)analogRead(V5) * VREF * ((DIV_R3 + DIV_R4) / DIV_R4) / 1024) + 0.33);
    loraSend(", ");
    loraSend(((float)analogRead(V3) * VREF * ((DIV_R5 + DIV_R6) / DIV_R6) / 1024) - (3.77 - 3.30));
    loraSendln(", 0");
  }
}