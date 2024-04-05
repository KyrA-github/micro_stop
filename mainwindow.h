#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <iostream>

#include <thread>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    HRESULT hr = S_OK;
    IMMDeviceEnumerator *enumerator = NULL;
    IMMDevice *device = NULL;
    IAudioEndpointVolume *endpointVolume = NULL;

    std::thread Thread;

    float volume;
    bool stopMicroThread = false;
    float volmeValue = 1.0f;

    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_progressBar_valueChanged(int value);
    void on_spinBox_valueChanged(int arg1);

private:
    void SetMicrophoneVolume(float level);
    float GetMicrophoneVolume();
    void edit_micro();
    bool thread_stop_m();

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
