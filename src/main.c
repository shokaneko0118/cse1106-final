#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <pigpio.h>
#define PIN 17

int LED = 0;

double calcavg(double readings[], int count) {
    double total = 0;
    for (int i = 0; i < count; i++) total += readings[i];
    return total / count;
}

void displayReadings(double readings[], int count, int status[]) {
    printf("Temperature    System Status    Warning LED\n");
    printf("0 = NORMAL or OFF, 1 = WARNING or ON\n\n");
    for (int i = 0; i < count; i++)
    {
        printf("%lf          %d                  %d\n", readings[i], status[i], status[i]);
    }
}

int isOutOfRange(double temperature) {
    return (temperature > 76 || temperature < 68) ? 1 : 0;
}

void checkReadings(double readings[], int count, int status[]) {
    for (int i = 0; i < count; i++) {
        if (isOutOfRange(readings[i]))
        {
            status[i] = 1;
        }
        else
        {
            status[i] = 0;
        }
    }
}

int testsRun = 0;
int testsPassed = 0;

void recordTest(const char *testName, int passed)
{
    testsRun++;
    if (passed) {
        testsPassed++;
        printf("PASS: %s\n", testName);
    } else {
        printf("FAIL: %s\n", testName);
    }
}

void test_T1(void) {
    double temperature = 75.0;
    recordTest("T1 75 F is within the acceptable range",
               isOutOfRange(temperature) == 0);
}

void test_T2(void) {
    double temperature = 64.0;
    recordTest("T2 64 F requires attention",
               isOutOfRange(temperature) == 1);
}

void test_T3(void) {
    double temperature = 76;
    recordTest("T3 76 F is within the acceptable range",
               isOutOfRange(temperature) == 0);
}

void test_T4(void) {
    double temperature = 79;
    recordTest("T4 79 F requires attention",
               isOutOfRange(temperature) == 1);
}

void test_T5(void) {
    double temperature = 50;
    recordTest("T5 50 F requires attention",
               isOutOfRange(temperature) == 1);
}

void runInitialTests(void) {
    printf("\nTest\n");

    test_T1();
    test_T2();
    test_T3();
    test_T4();
    test_T5();
}

int main(int argc, char* argv) {
    int COUNT;
    
    printf("Enter classroom temperature readings count\n");
    scanf("%d", &COUNT);
    double readings[COUNT + 1];
    int status[COUNT + 1];
    printf("\nEnter classroom temperature\n");

    for (int i = 0; i < COUNT; i++)
    {
        double x;
        printf("Reading\n");
        scanf("%lf", &x);
        readings[i] = x;    
    }

    printf("\nMonitor Start\n\n");

    printf("Average temperature: %lf F\n\n", calcavg(readings, COUNT));

    checkReadings(readings, COUNT, status);
    displayReadings(readings, COUNT, status);

    FILE *ptr = fopen("log.txt", "w");
    fprintf(ptr, "Temperature    System Status    Warning LED\n");
    fprintf(ptr, "0 = NORMAL or OFF, 1 = WARNING or ON\n\n");
    for (int i = 0; i < COUNT; i++)
    {
        fprintf(ptr, "%lf          %d                  %d\n", readings[i], status[i], status[i]);
    }
    fclose(ptr);

    runInitialTests();

    printf("TEST SUMMARY\n");
    printf("Tests passed: %d/%d\n", testsPassed, testsRun);
	
	testsRun = 0;
	testsPassed = 0;

    for (int i = 0; i < COUNT; i++)
    {
        if (status[i] == 1)
        {
            LED = 1;
            break;
        }
    }

    gpioInitialise();
    gpioSetMode(PIN, PI_OUTPUT);

    if (LED == 1)
    {
        gpioWrite(PIN, 1);
        sleep(10);
        gpioWrite(PIN, 0);
    }
    
    
    gpioTerminate();
    return 0;
}
