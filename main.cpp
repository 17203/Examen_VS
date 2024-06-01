
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;
using namespace sf;
//ronda.hpp
class Ronda {
private:
    int balas;
    int puntuacion;
    int enemigos;
    int cont;//contador de en que rondna va

public:
    Ronda();
    int disparaBala();
    void reiniciarBalas();
    void update();
    void drawTo(RenderWindow& window);
    void rondaTerminada();
    int getBalas() const; //para una variable no modificable ya que es constante
};
//ronda.cpp
Ronda::Ronda() {
    this->balas = 3;
    this->puntuacion = 0;
    this->enemigos = 0;
    this->cont = 0;
}

void Ronda::reiniciarBalas() {
    this->balas = 3;
}

void Ronda::update() {
    if (balas == 0 && enemigos == 0) {
        reiniciarBalas();
        puntuacion++;
        this->cont++;
    }
}

int Ronda::disparaBala() {
    if (this->balas > 0) {
        this->balas--;
    }
    return this->balas; //funcion int para regresar un valor y en caso de ser 0 no disparar más
}


void Ronda::rondaTerminada() {
    puntuacion++;
    reiniciarBalas();
    enemigos--;
    std::cout << "Ronda terminada" << std::endl; //no termina la roda D:
}

void Ronda::drawTo(RenderWindow& window) { //dibuja las balas de la ronda
    for (int i = 0; i < balas; i++) {// se usa un for ya que se modifica conforme se disparan
        RectangleShape shape;
        shape.setFillColor(Color::White);
        shape.setPosition(1150, 150 + i * 120);
        shape.setSize(Vector2f(30, 100));
        window.draw(shape);
    }
}

int Ronda::getBalas() const {
    return balas;
}
//Rrectangle.hpp
class Pato1 {
private:
    Vector2f speed;
    Vector2f acc;
    RectangleShape shape;
    int rebotesx;// ?
    int rebotesy;

public:
    bool vivo;
    Pato1(Vector2f size, RenderWindow& window);
    bool disparoAcertado(Vector2i position);
    void update();
    void drawTo(RenderWindow& window);
    int getRebotesY() const; //mismo que arriba
};
//Rrectangle.cpp
Pato1::Pato1(Vector2f size, RenderWindow& window) {
    shape = RectangleShape(size);
    shape.setPosition({ static_cast<float>(rand() % 1200), 940.0f });//manera fancy que puso chatgpt pq me estaba conflictuando mucho y generaba patos en coordenadas que no iban
    shape.setFillColor(Color::Red);
    if (rand() % 5 == 2) {
        speed = Vector2f(6.f, 6.f);
        shape.setFillColor(Color::Cyan);
    }
    else {
        speed = Vector2f(4.f, 4.f);
    }
    acc = Vector2f(0.f, 0.f);
    shape.setOutlineThickness(3);
    shape.setOutlineColor(Color::Cyan);
    rebotesx = 0;
    rebotesy = 0;
    vivo = true;
}

void Pato1::update() {
    speed -= acc;//no implemetado todavia
    shape.move(speed);
    if (shape.getPosition().x + shape.getSize().x >= 1200 || shape.getPosition().x < 0) {
        speed.x *= -1;
        shape.setOutlineColor(Color::White);
        rebotesx++;
    }
    if (vivo) {
        if (shape.getPosition().y + shape.getSize().y >= 1000 || shape.getPosition().y < 0) {
            speed.y *= -1;
            shape.setOutlineColor(Color::Green);
            rebotesy++;
        }
    }
    else if (shape.getPosition().y + shape.getSize().y >= 1000) {//para cuado se muera
        speed.y = 0;
    }
    if (rebotesy > 5 && shape.getPosition().y <= 0) {//si el pato ya reboto 6 veces y pego en otro lugar lo desaparece
        shape.setPosition(-1000, -1000);
    }
    if (vivo && rebotesy > 1 && shape.getPosition().y + shape.getSize().y >= 600) {//si el pato sigue vivo y ya dio 1 rebote, se agrega un limite para tener un area delimitada acorde al fondo
        rebotesy++;
        speed.y *= -1;
    }
}

void Pato1::drawTo(RenderWindow& window) {
    window.draw(shape);
}

bool Pato1::disparoAcertado(Vector2i position) {
    float x = shape.getPosition().x;
    float y = shape.getPosition().y;
    float w = shape.getSize().x;
    float h = shape.getSize().y;
    int mouseX = position.x;
    int mouseY = position.y;
    if (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h) {
        shape.setFillColor(Color::Green);
        vivo = false;
        speed.y = 4;
        speed.x = 0;
        return true;
    }
    else return false;
}

int Pato1::getRebotesY() const {
    return rebotesy;
}
//partida.hpp
class Partida {
private:
    int rondas;
    bool juegoSigue;
    int prob;

public:
    Partida(int r, bool JS);
    void jugar(RenderWindow& window);
};
//partida.cpp
Partida::Partida(int r, bool JS) {
    rondas = r;
    juegoSigue = JS;
    prob = rand() % 5;//probabilidad de un pato rapido
}

Ronda ron1;
Pato1* pato = nullptr; //se hace un apuntador para poder eliminarlo totalmente del sistema y que no se sature la memoria(lo agregue pq trababa mi laptop)
Partida partida(10, true);
bool gameStarted = false;//agregado para que cuando se pique a click derecho inicie el juego

void crearPato(RenderWindow& window);
//main.cpp
int main() {
    RenderWindow window(VideoMode(1200, 1000), "cazapato");
    window.setFramerateLimit(120);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Right && !gameStarted) {//comprueba si todavia no se a iniciado el juego, en caso de que si simplemente continuara
                    gameStarted = true;
                    std::cout << "inicio juego" << std::endl;
                }

                if (event.mouseButton.button == Mouse::Left && gameStarted && ron1.getBalas() > 0) {// si todavia se tienen balas
                    ron1.disparaBala();
                    if (pato != nullptr && pato->disparoAcertado(Mouse::getPosition(window))) {//comprueba si el pato existe y si el disparo acerto
                        ron1.rondaTerminada();
                        delete pato;
                        pato = nullptr;//desintegra al pato D:
                    }
                }
            }
        }

        window.clear();//limpia la ventana(no sabia que esto existia pero evita conflictos posteriores ya que al iniciar otra ronda las balas se sobreponian)

        if (gameStarted) {//cosas extrañas para que funcione
            if (pato != nullptr) {
                pato->update();
                pato->drawTo(window);
                if (pato->getRebotesY() >= 5) {
                    ron1.rondaTerminada();
                    delete pato;
                    pato = nullptr;
                }
            }
            else if (ron1.getBalas() > 0) {
                crearPato(window);
            }

            ron1.drawTo(window);
            ron1.update();
        }

        window.display();
    }

    return 0;
}

void crearPato(RenderWindow& window) {//crea patos y ya :D
    float x = float(50);
    float y = float(50);
    pato = new Pato1(Vector2f(x, y), window);
    pato->drawTo(window);
}
