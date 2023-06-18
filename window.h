#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QLabel>
#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <QtWidgets/QMessageBox>
#include <QPushButton>


class QPushButton;
class Window : public QWidget
{
public:
    explicit Window(QWidget *parent = 0);
private:
    QLabel *time_text;
    QLabel *moisture_text;
    QPushButton *b_pump;
    QPushButton *b_led;
    QPushButton *b_pump_on;
    QPushButton *b_led_on;
    QMqttClient *m_client;

    bool pump;
    bool led;
    bool pump_on;
    bool led_on;

private slots:

    void on_buttonPump_clicked();

    void on_buttonLed_clicked();
    void on_buttonPump_on_clicked();

    void on_buttonLed_on_clicked();

    void on_connect();

    void receive_message(const QByteArray &message, const QMqttTopicName &topic);
};

#endif // WINDOW_H
