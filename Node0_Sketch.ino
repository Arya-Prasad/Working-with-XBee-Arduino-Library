#include<XBee.h>

XBee xbee = XBee();

uint8_t payload[47] = {};



void setup() {

  Serial.begin(9600);
  
  xbee.setSerial(Serial);

  delay(1000);

}


void loop() {

  char data[][50] = {"1541731282,12345.6789,2017-10-03 15:20,40d7b9f2",
                     "2541731282,12345.6789,2017-10-03 15:20,40d7b9f2",
                     "3541731282,12345.6789,2017-10-03 15:20,40d7b9f2"};

  int row = sizeof(data)/sizeof(data[0]);

  send_csv_data (data, row);

  delay(50000); 

}


void send_csv_data (char datacsv[][50], int nrows) {
  
  for(int i = 0; i < nrows; i++) {
    
    XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40d7b9e8);
      
    ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
      
    for(int j = 0; datacsv[i][j]!='\0'; j++) {
      
      payload[j] = datacsv[i][j];
          
    }

    xbee.send(zbTx);
      
    delay(1000);
  }

  XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40d7b9e8);

  ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));

  payload[0] = '!';

  xbee.send(zbTx);
}

