
void encProcess()
{
  encValueA = digitalRead(encPinA);    // read encoder pins
  encValueB = digitalRead(encPinB);    //
  delay(1);

  if(encValueA == HIGH && encValueB == HIGH) {    // normal resting condition //

    if(encDirection == encRight) {           // right turn detected on previous loop
      encValue += encStep;                   //
      if(encValue > encMaxValue) {           //
        encValue=encMaxValue;                //
      }                                      //
    } else if (encDirection == encLeft) {    // left turn detected on previous loop
      encValue -= encStep;                   //
      if(encValue < encMinValue) {           //
        encValue = encMinValue;              //
      }                                      //
    }                                        //

    encDirection = encNoChange;    // normal resting action //

  } else {

    if(encValueA == LOW) {        // right turn detected
      encDirection = encRight;    //
    }                             //
    if(encValueB == HIGH) {       // left turn detected
      encDirection = encLeft;     //
    }                             //
    
  }
}
