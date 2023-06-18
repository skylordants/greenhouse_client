#include "window.h"

Window::Window(QWidget *parent) :
    QWidget(parent)
{
    // Set size of the window
    setFixedSize(900, 200);

    // Create and position the button
    time_text = new QLabel(this);
    time_text->setText("Last Report: ");
    time_text->setGeometry(10, 0, 350, 20);

    moisture_text = new QLabel(this);
    moisture_text->setText("Moisture: ");
    moisture_text->setGeometry(10, 20, 350, 20);

    b_pump = new QPushButton("Pump", this);
    b_pump->setGeometry(10, 60, 100, 20);

    b_led = new QPushButton("LED", this);
    b_led->setGeometry(120, 60, 100, 20);

    b_pump_on = new QPushButton("Pump on", this);
    b_pump_on->setGeometry(10, 100, 100, 20);

    b_led_on = new QPushButton("LED on", this);
    b_led_on->setGeometry(120, 100, 100, 20);

    m_client = new QMqttClient(this);
    m_client->setHostname("g4.yt");
    m_client->setPort(1883);
    m_client->setUsername("greenhouse");
    m_client->setPassword("greenhouse");

    connect(m_client,&QMqttClient::connected, this, &Window::on_connect);
    connect(m_client, &QMqttClient::messageReceived, this, &Window::receive_message);

    m_client->connectToHost();

    connect(b_pump, &QPushButton::clicked, this, &Window::on_buttonPump_clicked);
    connect(b_led, &QPushButton::clicked, this, &Window::on_buttonLed_clicked);
    connect(b_pump_on, &QPushButton::clicked, this, &Window::on_buttonPump_on_clicked);
    connect(b_led_on, &QPushButton::clicked, this, &Window::on_buttonLed_on_clicked);

    led = false;
    pump = false;
    led_on = false;
    pump_on = false;
}




void Window::on_buttonPump_clicked()
{
    QMqttTopicName o {"greenhouse/control/override/pump"};

    QByteArray msg {pump ? "0" : "1"};
    pump = !pump;
    m_client->publish(o, msg);
}

void Window::on_buttonLed_clicked()
{
    QMqttTopicName o {"greenhouse/control/override/led"};

    QByteArray msg {led ? "0" : "1"};
    led = !led;
    m_client->publish(o, msg);
}

void Window::on_buttonPump_on_clicked()
{
    QMqttTopicName c {"greenhouse/control/pump"};

    QByteArray msg {pump_on ? "0" : "1"};
    pump_on = !pump_on;
    m_client->publish(c, msg);
}

void Window::on_buttonLed_on_clicked()
{
    QMqttTopicName c {"greenhouse/control/led"};

    QByteArray msg {led_on ? "0" : "1"};
    led_on = !led_on;
    m_client->publish(c, msg);
}


void Window::on_connect() {
    QMqttTopicFilter filter {"greenhouse/#"};

    auto subscription = m_client->subscribe(filter);
    if (!subscription) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
    }
}

void Window::receive_message(const QByteArray &message, const QMqttTopicName &topic) {
    if (topic.name() == "greenhouse/report_time") {
        const QString content = QLatin1String("Last Report: ") + message;
        time_text->setText(content);
        return;
    }
    if (topic.name() == "greenhouse/measurements/soil_moisture") {
        const QString content = QLatin1String("Moisture: ") + message;
        moisture_text->setText(content);
    }
}
