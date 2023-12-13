#include <Arduino.h>
#include <Config.h>

int count1,count2 = 0;

void toggleLED(void * parameter){
  for(;;){ 
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);//Pause the task for 500ms
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);//Pause the task again for 500ms
  }
}
void uploadToAWS(void * parameter){
    // Implement your custom logic here
    // When you're done, call vTaskDelete. Don't forget this!
    vTaskDelete(NULL);
}
// This TaskHandle will allow 
TaskHandle_t task1Handle = NULL;
void task1(void * parameter){
  for(;;){count1++;
    // Serial.print("Task 1 counter"); Serial.println(count1);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
// This TaskHandle will allow 
TaskHandle_t task2Handle = NULL;
void task2(void * parameter){
  for(;;){count2++;
    // Serial.print("Task 2 counter");Serial.println(count2);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void RTOS_Setup() {
  //............... Setup RTOS xTaskCreate ................//
  xTaskCreate(task1, "Task 1", 1000, NULL, 1, &task1Handle);
  xTaskCreate(task2, "Task 2", 1000, NULL, 1, &task2Handle);
  // Function that should be called// Name of the task (for debugging)// Stack size (bytes)// Parameter to pass// Task priority// Task handle
  xTaskCreate(toggleLED, "Toggle LED", 1000, NULL, 1, NULL);  
  xTaskCreate(uploadToAWS, "Upload to AWS", 1000, NULL, 1, NULL);
  // vTaskDelete(NULL);
}