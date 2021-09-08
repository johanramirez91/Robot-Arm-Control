#include "mbed.h" //Mbed library to embeded card NucletoSTM32 l432KC

Serial pc(USBTX, USBRX);

//P.pulsewidth(0.002); Abierto
//P.pulsewidth(0.00057); Cerrado

DigitalOut BRDIR(D2);
DigitalOut BLDIR(D4);
DigitalOut MLDIR(D10);
DigitalOut PRDIR(D11);

PwmOut BR(D0);  //D0
PwmOut BL(D1);  //D1
PwmOut ML(D3);  //D3
PwmOut PR(D6);  //D6
PwmOut B(PA_1); //A1
PwmOut P(PA_3); //A2

AnalogIn Sen1(A0);
AnalogIn Sen2(A3);
AnalogIn Sen3(A5);
AnalogIn Sen4(A6);

// Cafe GND
// Rojo Medio
// Naranja VCC

double Sensor1, Sensor2, Sensor3, Sensor4, kpB, y1, e1, u1, y2, e2, u2, kpML, y3, e3, u3, kpPR, y4, e4, u4, Base, SetBase, SetMedio, SetPeak, PWMB, PWMBR, PWMBL, PWMML, PWMPR;

char dato[5];

bool Abrir;

void ConB()
{
    PWMB = 0.00000000045724737083 * Base * Base * Base - 0.00000012345679012346 * Base * Base - 0.00000092592592592484 * Base + 0.00225;
    B.pulsewidth(PWMB);
}

void ConP()
{
    if (Abrir == 1)
    {
        P.pulsewidth(0.002);
    }
    else
    {
        P.pulsewidth(0.0005);
    }
}

void ConBR()
{
    kpB = 20;

    Sensor1 = Sen1.read_u16();
    Sensor1 = Sensor1 * 0.000050354; // Conversor Motor Base Derecha

    y1 = 64.286 * Sensor1 - 24.017;

    e1 = SetBase - y1; //Error Motor Base Derecha
    u1 = kpB * e1;     //Accion de Control Motor Base Derecha

    if (y1 < SetBase)
    { //Motores Derechos
        BRDIR = 0;
    }
    else
    {
        BRDIR = 1;
    }

    PWMBR = u1 / 270.0;
    if (PWMBR < 0.0)
    {
        PWMBR = -1 * PWMBR;
    }
    if (PWMBR > 1.0)
    {
        PWMBR = 1.0;
    }

    BR.write(PWMBR);
    pc.printf("Sensor 1: %f\n\r", Sensor1);
    pc.printf("Salida 1 %f\n\r", y1);
    pc.printf("Error 1: %f\n\r", e1);
    pc.printf("PWM 1: %f\n\r", PWMBR);
}

void ConBL()
{
    kpB = 20;

    Sensor2 = Sen2.read_u16();
    Sensor2 = Sensor2 * 0.000050354; // Conversor Motor Base Derecha

    y2 = -65.445 * Sensor2 + 186.35;

    e2 = SetBase - y2;   //Error Motor Base Derecha
    u2 = kpB * e2 - 7.0; //Accion de Control Motor Base Derecha

    if (y2 < SetBase)
    { //Motores Izquierdos
        BLDIR = 1;
    }
    else
    {
        BLDIR = 0;
    }

    PWMBL = u2 / 360.0;
    if (PWMBL < 0.0)
    {
        PWMBL = -1 * PWMBL;
    }
    if (PWMBL > 1.0)
    {
        PWMBL = 1.0;
    }

    BL.write(PWMBL);
    pc.printf("Sensor 2: %f\n\r", Sensor2);
    pc.printf("Salida 2 %f\n\r", y2);
    pc.printf("Error 2: %f\n\r", e2);
    pc.printf("PWM 2: %f\n\r", PWMBL);
}

void ConML()
{
    kpML = 20;

    Sensor3 = Sen3.read_u16();
    Sensor3 = Sensor3 * 0.000050354; // Conversor Motor Base Derecha

    y3 = -63.951 * Sensor3 + 195.85;

    e3 = SetMedio - y3; //Error Motor Base Derecha
    u3 = kpML * e3;     //Accion de Control Motor Base Derecha

    if (y3 < SetMedio)
    { //Motores Izquierdos
        MLDIR = 1;
    }
    else
    {
        MLDIR = 0;
    }

    PWMML = u3 / 360.0;
    if (PWMML < 0.0)
    {
        PWMML = -1 * PWMML;
    }
    if (PWMML > 1.0)
    {
        PWMML = 1.0;
    }

    ML.write(PWMML);
    pc.printf("Salida ML: %f\n\r", Sensor3);
    pc.printf("Salida 3: %f\n\r", y3);
    pc.printf("Error 3: %f\n\r", e3);
    pc.printf("PWM 3: %f\n\r", PWMML);
}

void ConPR()
{
    kpPR = 15;

    Sensor4 = Sen4.read_u16();
    Sensor4 = Sensor4 * 0.000050354; // Conversor Motor Base Derecha

    y4 = 58.845 * Sensor4 - 6.1271;

    e4 = SetPeak - y4; //Error Motor Base Derecha
    u4 = kpPR * e4;    //Accion de Control Motor Base Derecha

    if (y4 < SetPeak)
    { //Motores Izquierdos
        PRDIR = 0;
    }
    else
    {
        PRDIR = 1;
    }

    PWMPR = u4 / 360.0;
    if (PWMPR < 0.0)
    {
        PWMPR = -1 * PWMPR;
    }
    if (PWMML > 1.0)
    {
        PWMPR = 1.0;
    }

    PR.write(PWMPR);
    pc.printf("%f\n\r", Sensor4);
    pc.printf("Salida PR: %f\n\r", y4);
    pc.printf("Error PR: %f\n\r", e4);
    pc.printf("PWM PR: %f\n\r", PWMPR);
}

int main()
{
    pc.baud(230400);

    BR.period_ms(1);
    BL.period_ms(1);
    ML.period_ms(1);
    PR.period_ms(1);
    B.period(0.020);
    P.period(0.020);

    dato[0] = 90.0;
    dato[1] = 90.0;
    dato[2] = 90.0;
    dato[3] = 90.0;
    dato[4] = 0;

    printf("\nIniciando envio\n");

    while (1)
    {
        if (pc.readable())
        {
            for (int i = 0; i < 5; i++)
            {
                dato[i] = pc.getc();
                pc.printf("%c", dato[i]);
            }
        }
        Base = dato[0];
        SetBase = dato[1];
        SetMedio = dato[2];
        SetPeak = dato[3];
        Abrir = dato[4];
        ConB();
        ConBR();
        ConBL();
        ConML();
        ConPR();
        ConP();
    }
}