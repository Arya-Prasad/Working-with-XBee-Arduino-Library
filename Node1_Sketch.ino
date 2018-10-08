#include <XBee.h>

XBee xbee = XBee();
  
XBeeResponse response = XBeeResponse();

ZBRxResponse rx = ZBRxResponse();               //create a response object

char rx_buffer[30][50] = {};
  
char send_buffer[][50] = {};
  
int r_count, total_datapts = 0;

char data_own[][50] = {"1541731282,12345.6789,2017-10-03 15:20,40d7b9e8",
                       "2541731282,12345.6789,2017-10-03 15:20,40d7b9e8",
                       "3541731282,12345.6789,2017-10-03 15:20,40d7b9e8"};

int data_count = sizeof(data_own)/sizeof(data_own[0]);

uint8_t payload[47] = {};

  
void setup() {

  Serial.begin(9600);
  
  xbee.setSerial(Serial);

  Serial.println("Starting up!");
  
  delay(1000);                                  //delay for XBee startup and association          

}


void loop() {

xbee.readPacket();                           //continuously reads packets, looking for ZB Receive

if (xbee.getResponse().isAvailable()) {        //got some response       
      
  if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {       //got a zb rx packet
     
     xbee.getResponse().getZBRxResponse(rx);                 // now fill our zb response class         

     if (rx.getData()[0]!= '!') {
            
        receive_csv_data();
     }
     else if (rx.getData()[0] = '!') {
 
        for (int i = 0; i < rx.getDataLength(); i++) {       //discard the bytes that follows
              
          byte discard = rx.getData()[i];
        }
        
        append_csv_data();

        send_csv_data(send_buffer,total_datapts);
      }  
  }
}
}    

//----------------------------receive_csv_data---------------------------//

void receive_csv_data() {
  for (int i = 0; i < rx.getDataLength(); i++) {            //loop to get the frame payload                                      
   
    rx_buffer[r_count][i] = rx.getData()[i];
  }
  r_count++;
}


//---------------------------------append_csv_data------------------------------//

void append_csv_data() {
    
  for (int i = 0; i < data_count; i++) {       //merge the arrays, store in send_buffer
        
    char str[50] = {};       
    strcpy(str,rx_buffer[i]);
    delay(10);
    strcpy(send_buffer[i],str);      
    delay(10);
  }

  total_datapts = data_count+r_count; 
         
  for (int i = 0; i < data_count; i++) { 
          
     char str[50] = {};
     int pos = i+r_count;       
     strcpy(str,data_own[i]);
     delay(10);         
     strcpy(send_buffer[pos],str);
     delay(10);
  }
  
  r_count = 0;
}


//---------------------------send_csv_data------------------------------//

void send_csv_data (char datacsv[][50], int nrows) {   
    
 for(int i = 0; i < nrows; i++) {
   
   XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x414f7f0f);

   ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));

   for(int j = 0; datacsv[i][j]!='\0'; j++) {
     
     payload[j] = datacsv[i][j];      
   }
   delay(1500);
   
   xbee.send(zbTx);
 }
 XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x414f7f0f);

 ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
      
 delay(100);
      
 payload[0] = '!';

 xbee.send(zbTx);
}

