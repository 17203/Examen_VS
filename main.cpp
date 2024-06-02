
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

Ronda ron1;
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
    int getPositionY();
    void drawTo(RenderWindow& window);
    int tamanoY();
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
    speed -= acc; // no implementado todavía
    shape.move(speed);

    if (shape.getPosition().x + shape.getSize().x >= 1200 || shape.getPosition().x < 0) {//que no salga de los ejes x
        speed.x *= -1;
        shape.setOutlineColor(Color::White);
        rebotesx++;
    }

    if (vivo) {
        if (shape.getPosition().y + shape.getSize().y >= 1000 || shape.getPosition().y < 0) {//aumenta los rebotes y hace que suba
            speed.y *= -1;
            shape.setOutlineColor(Color::Green);
            rebotesy++;
        }
    }
    else if (shape.getPosition().y + shape.getSize().y >= 1000) {
        speed.y = 0;
        // La lógica de terminar la ronda se maneja en el main
    }

    if (vivo && rebotesy > 1 && shape.getPosition().y + shape.getSize().y >= 600) {//se agrego para que no se mueva por toda la patalla el pato
        speed.y *= -1;
    }
}

void Pato1::drawTo(RenderWindow& window) {
    window.draw(shape);
}
int Pato1::getPositionY(){//se agrego para manipular codigo más facil
return shape.getPosition().y; 
}
int Pato1::tamanoY() {//se agrego para manipular codigo más facil
    return shape.getSize().y;
}
bool Pato1::disparoAcertado(Vector2i position) {
    float x = shape.getPosition().x;
    float y = shape.getPosition().y;
    float w = shape.getSize().x;
    float h = shape.getSize().y;
    int mouseX = position.x;
    int mouseY = position.y;
    if (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h) {
        shape.setFillColor(Color::Green);//esto maneja la logica para que caiga el pato
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


Pato1* pato = nullptr; //se hace un apuntador para poder eliminarlo totalmente del sistema y que no se sature la memoria(lo agregue pq trababa mi laptop)
Partida partida(10, true);
bool gameStarted = false;//agregado para que cuando se pique a click derecho inicie el juego

void crearPato(RenderWindow& window);
//main.cpp
int main() {
    RenderWindow window(VideoMode(1200, 1000), "cazapato");
    window.setFramerateLimit(120);

    Texture backgroundTexture;
<<<<<<< HEAD
    if (!backgroundTexture.loadFromFile("texturas/Fondo.png")) {
=======
    if (!backgroundTexture.loadFromFile("Fondo.png")) {
>>>>>>> 27137b1d0c235ba186f687c0e71882783c1c0e9b
        return EXIT_FAILURE;
    }
    Sprite backgroundSprite(backgroundTexture);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Right && !gameStarted) {//comprueba si todavia no se a iniciado el juego, en caso de que si simplemente continuara
                    gameStarted = true;
                    cout << "inicio juego" << std::endl;
                }

                if (event.mouseButton.button == Mouse::Left && gameStarted && ron1.getBalas() > 0) {// si todavia se tienen balas
                    ron1.disparaBala();
                    if (pato != nullptr && pato->disparoAcertado(Mouse::getPosition(window))) {//comprueba si el pato existe y si el disparo acerto
                        pato->vivo = false;//lo marca como muerto 
                    }
                }
            }
        }

        window.clear();//limpia la ventana(no sabia que esto existia pero evita conflictos posteriores ya que al iniciar otra ronda las balas se sobreponian)
        window.draw(backgroundSprite);
        if (gameStarted) {//cosas extrañas para que funcione
            if (pato != nullptr) {
                pato->update();
                pato->drawTo(window);
                if (pato->getRebotesY() == 5) {//mismo que abajo pero con rebotes en la parte superior de la pantalla
                    ron1.rondaTerminada();
                    delete pato;
                    pato = nullptr;//lo desintegra
                }
                else if (!pato->vivo && pato->getPositionY() + pato->tamanoY() >= 1000) {//checa si toco el piso, en caso de que si inicia nueva ronda
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
