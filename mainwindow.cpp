#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Инициализация COM
    hr = CoInitialize(NULL);

    // Создание экземпляра перечислителя устройств
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
                          __uuidof(IMMDeviceEnumerator), (LPVOID *)&enumerator);

    // Получение устройства микрофона по умолчанию
    hr = enumerator->GetDefaultAudioEndpoint(eCapture, eCommunications, &device);

    // Получение интерфейса для управления громкостью
    hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);

    ui->setupUi(this);
    Thread = std::thread([this](){ edit_micro(); });
}

void MainWindow::edit_micro()
{
    while (true) {
        if (thread_stop_m())
        {
            volume = GetMicrophoneVolume();
            std::cout << volmeValue<< std::endl;
            std::cout << volume << std::endl;
            if (volume != volmeValue)
            {
                SetMicrophoneVolume(volmeValue);
                //cout << "Microphone volume is: " << volume << endl;
            }
        }
    }
}

MainWindow::~MainWindow()
{
    stopMicroThread = false;
    if (enumerator)
        enumerator->Release();
    if (device)
        device->Release();
    if (endpointVolume)
        endpointVolume->Release();

    CoUninitialize();
    delete ui;
}


// В функциях on_pushButton_2_clicked() и on_pushButton_clicked() меняем значение атомарной переменной в зависимости от действий пользователя
void MainWindow::on_pushButton_2_clicked()
{
    // Создаем поток, передавая лямбда-выражение

    stopMicroThread = true;
}

void MainWindow::on_pushButton_clicked()
{
    stopMicroThread = false;

}


void MainWindow::SetMicrophoneVolume(float level) {
    hr = endpointVolume->SetMasterVolumeLevelScalar(level, NULL);
}

float MainWindow::GetMicrophoneVolume() {
    float level;
    hr = endpointVolume->GetMasterVolumeLevelScalar(&level);
    return level;
}

void MainWindow::on_progressBar_valueChanged(int value) {
    // ваш код обработки события изменения значения прогресс-бара
}
bool MainWindow::thread_stop_m(){
    return stopMicroThread;
}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    stopMicroThread = false;
    volmeValue = static_cast<float>(arg1) + 0.1f;
    std::cout << volmeValue << std::endl;
}

