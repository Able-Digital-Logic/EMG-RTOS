#include <Arduino.h>
#include "freertos/FreeRTOS.h" //free rtos stuff
#include "freertos/task.h" //task control stuff
#include "soc/adc_channel.h" //adc socket, macros to include adc stuff
//===Declerations===

//Pins
const uint8_t EMG1Pin=A1;
const uint8_t EMG2Pin=A2;
const uint8_t MotorPin=D10; 
  //UART LATER RX=D7 (GPIO20), TX=D6 (GPIO21)
//emg stuff
const uint32_t TimerHz = 1000000;
const uint32_t EMGSampleRate = 2000;
const uint32_t SamplePeriodUs = TimerHz / EMGSampleRate;
const int EnvWindow=100;

//emg stuff to be calibrated later
const uint16_t EMGRest = 30; //find where these intial values came from
const uint16_t EMGFull = 600;

//Moto PWM stuff
const int PWMHz = 20000;
const int PWMBits = 10;


struct EMGSample{
  uint16_t ch1;
  uint16_t ch2;
};


// put function declarations here:
void EMGReadInit(){
  //need to set each pin we use as an ADC input
  //4 pins can be set to GPIO Analogue input, so we use those
  //A0, A1, A2, A3. Other pins can be set to analogue i think as well though but more complicated.
    //gpio_num_t pin = (gpio_num)(int)arg;
  //A1 and A2 to start, A0 is strapping, A3 is warned to stop working. Avoid A0 for sure, but test A3
  analogReadResolution(12);
  analogSetPinAttenuation(A1, ADC_11db);
  analogSetPinAttenuation(A2, ADC_11db);
  //analogContinuousSetWidth(A1, 12);
  //analogContinuousSetWidth(A2, 12); //not sure how to set which pin to set this for
  //configadc(adc1, ADC1_GPIO1_CHANNEL, ADC_11db, ADC_WIDTH_12Bit);
  //configadc(adc1, ADC1_GPIO2_CHANNEL, ADC_11db, ADC_WIDTH_12Bit);
  //adc1Start();
}
void MotorOutInit(){
//probably PWM, D10
  ledcAttach(MotorPin, PwmHz, PwmBits);
  ledcWrite(MotorPin, 0); 
}

void UARTInit(){
  Serial.setTxPin(GPIO_NUM_21); //Pin D6
  Serial.setTxBufferSize(256);
  Serial.setRxPin(GPIO_NUM_20); //Pin D7
  Serial.setRXBufferSize(256);
  Serial.Begin(115200);
}

void setup(){
  Serial.begin(115200);
  UARTInit();
  MotorOutInit();

  emgQueue = xQueueCreate(64, sizeof(EMGSample));

}
void appmain(){
//set the schedule. Round Robin with input and output being top and second priority. digital signals should be least priority.
}
