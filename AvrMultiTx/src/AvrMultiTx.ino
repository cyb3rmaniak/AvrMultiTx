#include "View.h"
#include "Input.h"

#define CHANNEL_COUNT 8
#define CHANNEL_MIN_VALUE 1000
#define CHANNEL_MAX_VALUE 2000
#define CHANNEL_DEFAULT_VALUE 1500

#ifdef TX_ENABLED
    #define GAP_LENGTH 7000  // Set the PPM gap length between two frames in microseconds (1ms = 1000µs)
    #define FRAME_LENGTH 22500  // Set the PPM frame length in microseconds (1ms = 1000µs)
    #define PULSE_LENGTH 300  // Set the pulse length
    #define PIN_HIGH 1  // Set polarity of the pulses: 1 is positive, 0 is negative
    #define PIN_LOW 0  // Set polarity of the pulses: 1 is positive, 0 is negative
    #define OUTPUT_PIN 5  // Set PPM signal output pin on the arduino
    #define FIXED_TIMER_COMPARE_DELAY 6 // Manual offset to be more precise with FrSky receivers
#endif

#define MAPPING_YAW         0
#define MAPPING_THROTTLE    1
#define MAPPING_PITCH       2
#define MAPPING_ROLL        3

#define PS4_MAPPING_L_STICK_X MAPPING_YAW
#define PS4_MAPPING_L_STICK_Y MAPPING_THROTTLE
#define PS4_MAPPING_R_STICK_X MAPPING_ROLL
#define PS4_MAPPING_R_STICK_Y MAPPING_PITCH

const uint16_t DELAY_BETWEEN_INPUT_UPDATES[] = { 15, 15 }; // In milliseconds
const uint16_t DELAY_BETWEEN_DRAWS[]         = { 90, 90 }; // In milliseconds

int curr_values[CHANNEL_COUNT];

uint32_t    _currTime;
uint32_t    _nextDrawTime;
uint32_t    _nextInputTime;

Settings settings;
View view;
Input input;

#if TX_ENABLED
    // this array holds the values that will be sent in the the ppm signal.
    int ppm_outbox[CHANNEL_COUNT];
#endif

void setup()
{
    Serial.begin(115200);
    Serial.println("FPS_TX Initializing...");

    settings.Initialize();
    view.Initialize(CHANNEL_COUNT, &settings);
    input.Initialize(&view, &settings);

    // Initiallize default ppm values
    for(int i = 0 ; i < CHANNEL_COUNT ; i++)
        curr_values[i] = CHANNEL_DEFAULT_VALUE;

#ifdef TX_ENABLED
    StartTransmitting();
#endif
}

void loop()
{
    _currTime = millis();

    if (_currTime >= _nextDrawTime)
    {
        view.Refresh();
        _nextDrawTime = _currTime + DELAY_BETWEEN_DRAWS[view.currScreen];
    }

    if (_currTime >= _nextInputTime)
    {
        input.ReadFromController(_currTime);
        _nextInputTime = _currTime + DELAY_BETWEEN_INPUT_UPDATES[view.currScreen];
    }

    delay(10); // Give stuff time to happen, man... Just chill...
}

#ifdef TX_ENABLED

void StartTransmitting()
{
    for(int i = 0 ; i < CHANNEL_COUNT ; i++)
        ppm_outbox[i] = CHANNEL_DEFAULT_VALUE;

    // Setup the PPM output pin
    pinMode(OUTPUT_PIN, OUTPUT);
    digitalWrite(OUTPUT_PIN, PIN_LOW);  //set the PPM signal pin to the default state (off)

    // Stuff to send out the PPM pulses...
    cli();
    TCCR1A = 0; // set entire TCCR1 register to 0
    TCCR1B = 0;

    OCR1A = 999 - FIXED_TIMER_COMPARE_DELAY;  // compare match register, change this
    TCCR1B |= (1 << WGM12);  // turn on CTC mode
    TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
    sei();
}

// Timer interrupt to send PPM
ISR(TIMER1_COMPA_vect)
{
    static boolean state = true;

    TCNT1 = 0;

    if (state) 
    {
        //start pulse
        digitalWrite(OUTPUT_PIN, PIN_HIGH);
        OCR1A = PULSE_LENGTH * 2 - FIXED_TIMER_COMPARE_DELAY;
        state = false;
    } 
    else
    {  
        //end pulse and calculate when to start the next pulse
        static byte curr_channel;
        static unsigned int calc_rest;

        digitalWrite(OUTPUT_PIN, PIN_LOW);  
        state = true;

        if (curr_channel >= CHANNEL_COUNT)
        {
            curr_channel = 0;
            calc_rest = calc_rest + PULSE_LENGTH;
            OCR1A = (FRAME_LENGTH - calc_rest) * 2 - FIXED_TIMER_COMPARE_DELAY;
            calc_rest = 0;
        }
        else
        {
            OCR1A = (ppm_outbox[curr_channel] - PULSE_LENGTH) * 2 - FIXED_TIMER_COMPARE_DELAY;
            calc_rest = calc_rest + ppm_outbox[curr_channel];
            curr_channel++;
        }  
    }
}

#endif