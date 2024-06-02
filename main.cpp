
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
    this->enemigos = 0;
    this->cont = 0;
}

void Ronda::reiniciarBalas() {
    this->balas = 3;
}

void Ronda::update() {
    if (balas == 0 && enemigos == 0) {
        reiniciarBalas();
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
    reiniciarBalas();
    enemigos--;
    cout << "Ronda terminada" << endl; //no termina la roda D:
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
//partida.hpp
class Partida {
private:
    int rondas;
    int vuelo;
    int prob;
    int patosVolados; // contador de patos volados
    int score;
public:
    bool juegoSigue;
    Partida(int r, bool JS, int p);
    void jugar(RenderWindow& window);
    void patosFuera();//la partida termina si llega a 3
    void incrementarPatosVolados(); // nueva función para incrementar patos volados
    void reiniciarPatosVolados(); // nueva función para reiniciar patos volados
    int getPatosVolados() const; // nueva función para obtener patos volados
    void drawPatosVolados(RenderWindow& window); // nueva función para dibujar cuadros amarillos
    int puntuacion();
    void scoreIncrease();
    int scoreFinal();
};

//partida.cpp
Partida::Partida(int r, bool JS, int p) {
    rondas = r;
    juegoSigue = JS;
    vuelo = p;
    prob = rand() % 5;//probabilidad de un pato rapido
    this->patosVolados = 0;
    this->score = 0;
}

void Partida::patosFuera() {
    vuelo++;
    if (vuelo == 3) {
        juegoSigue = false;
        cout << "Juego terminado. Se escaparon 3 patos." << endl;
        this->vuelo = 0;
        puntuacion();
    }
}
void Partida::scoreIncrease() {
    score++;
}
int Partida::scoreFinal() {
    score = score*250;
    return score;
}
void Partida::incrementarPatosVolados() {
    patosVolados++;
}

void Partida::reiniciarPatosVolados() {
    patosVolados = 0;
}

int Partida::getPatosVolados() const {
    return patosVolados;
}

void Partida::drawPatosVolados(RenderWindow& window) {
    for (int i = 0; i < patosVolados; i++) {
        RectangleShape shape;
        shape.setFillColor(Color::Yellow);
        shape.setPosition(20, 850 + i * 40); // ajustar posición para la parte inferior izquierda
        shape.setSize(Vector2f(30, 30));
        window.draw(shape);
    }
}
int Partida::puntuacion() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "cierre esta ventana para volver a jugar");
    Font font;
    if (!font.loadFromFile("fuentes/Pixel.ttf")) {
        return EXIT_FAILURE;
    }

    Texture puntuacionTexture;
    if (!puntuacionTexture.loadFromFile("texturas/puntuacion.png")) {
        return EXIT_FAILURE;
    }
    Sprite puntuacionSprite(puntuacionTexture);
    puntuacionSprite.setScale(
        window.getSize().x / static_cast<float>(puntuacionTexture.getSize().x),
        window.getSize().y / static_cast<float>(puntuacionTexture.getSize().y)
    );

    Text text;
    text.setFont(font);
    text.setString(to_string(scoreFinal()));//funcion que no sabia que existia, convierte los datos que recibe a el dato que necesites
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    text.setPosition(370.f, 250.f);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(puntuacionSprite);
        // Mostrar el puntaje final
        window.draw(text);

        window.display();
    }

    return EXIT_SUCCESS;
}
Partida partida(10, false, 0); //mover todo hacia arriba, se necesitan rondas para que la partida funcione y partidas para que los patos funcionen en referencia a estos
//Rrectangle.hpp
class Pato1 {
private:
    Vector2f speed;
    Vector2f acc;
    Texture textureAlive;
    Texture textureLD;
    Texture textureLU;
    Texture textureDD;
    Texture textureDead;
    Sprite spriteVivo;
    Sprite spriteLD;
    Sprite spriteLU;
    Sprite spriteDD;
    Sprite spriteMuerto;
    int rebotesx;// ?
    int rebotesy;

public:
    bool vivo;
    Pato1(Vector2f size, RenderWindow& window);
    bool disparoAcertado(Vector2i position);
    void update();
    void direccion();
    int getPositionY();
    void drawTo(RenderWindow& window);
    int tamanoY();
    int getRebotesY() const; //mismo que arriba
};
//Rrectangle.cpp
Pato1::Pato1(Vector2f size, RenderWindow& window) {
    spriteVivo.setPosition({ static_cast<float>(rand() % 1200), 890.0f });//manera fancy que puso chatgpt pq me estaba conflictuando mucho y generaba patos en coordenadas que no iban

    if (!textureAlive.loadFromFile("texturas/PatoDU.png")) {
        cout << "Error loading pato_vivo.png" << endl;
    }
    if (!textureLD.loadFromFile("texturas/PatoDU.png")) {
        cout << "Error loading pato_vivo.png" << endl;
    }
    if (!textureLD.loadFromFile("texturas/PatoLD.png")) {
        cout << "Error loading pato_vivo.png" << endl;
    }
    if (!textureLU.loadFromFile("texturas/PatoLU.png")) {
        cout << "Error loading pato_vivo.png" << endl;
    }
    if (!textureDD.loadFromFile("texturas/PatoDD.png")) {
        cout << "Error loading pato_vivo.png" << endl;
    }
    if (!textureDead.loadFromFile("texturas/pato_muerto.png")) {
        cout << "Error loading pato_muerto.png" << endl;
    }

    spriteVivo.setTexture(textureAlive);
    spriteLD.setTexture(textureLD);
    spriteLU.setTexture(textureLU);
    spriteDD.setTexture(textureDD);
    spriteMuerto.setTexture(textureDead);

    float scaleX = size.x / textureAlive.getSize().x;
    float scaleY = size.y / textureAlive.getSize().y;
    spriteVivo.setScale(scaleX, scaleY);
    spriteLD.setScale(scaleX, scaleY);
    spriteLU.setScale(scaleX, scaleY);
    spriteDD.setScale(scaleX, scaleY);
    spriteMuerto.setScale(scaleX, scaleY);

    if (rand() % 5 == 2) {
        speed = Vector2f(6.f, 6.f);
    }
    else {
        speed = Vector2f(4.f, 4.f);
    }
    acc = Vector2f(0.f, 0.f);
    rebotesx = 0;
    rebotesy = 0;
    vivo = true;
}

void Pato1::direccion() {//cambia el sprite en funcion de la direccionn del pato
    if (speed.x > 0 && speed.y > 0) {//vectores D:
        spriteVivo.setTexture(textureDD);
    }
    else if (speed.x > 0 && speed.y < 0) {
        spriteVivo.setTexture(textureAlive);
    }
    else if (speed.x < 0 && speed.y < 0) {
        spriteVivo.setTexture(textureLU);
    }
    else if (speed.x < 0 && speed.y > 0) {
        spriteVivo.setTexture(textureLD);
    }
}

void Pato1::update() {
    speed -= acc; // no implementado todavía
    spriteVivo.move(speed); // Mover el sprite en lugar de la forma

    // Revisar los límites del sprite escalado
    if (spriteVivo.getPosition().x + spriteVivo.getGlobalBounds().width >= 1200 || spriteVivo.getPosition().x < 0) {
        speed.x *= -1;
        rebotesx++;
    }

    if (vivo) {
        if (spriteVivo.getPosition().y + spriteVivo.getGlobalBounds().height >= 1000 || spriteVivo.getPosition().y < 0) {
            speed.y *= -1;
            rebotesy++;
        }
    }
    else if (spriteVivo.getPosition().y + spriteVivo.getGlobalBounds().height >= 1000) {
        speed.y = 0;
        // La lógica de terminar la ronda se maneja en el main
    }

    if (vivo && rebotesy > 1 && spriteVivo.getPosition().y + spriteVivo.getGlobalBounds().height >= 600) {
        speed.y *= -1;
    }
    if (!vivo && getPositionY() + tamanoY() < 1000) {
        acc.y = 0.5f;
        speed += acc;
        this->spriteMuerto.move(0, speed.y);
    }
}

void Pato1::drawTo(RenderWindow& window) {
    if (vivo) {
        window.draw(spriteVivo);
    }
    else {
        window.draw(spriteMuerto);
    }
}
int Pato1::getPositionY() {
    return spriteVivo.getPosition().y;
}
int Pato1::tamanoY() {
    return spriteVivo.getTexture()->getSize().y;
}
bool Pato1::disparoAcertado(Vector2i position) {
    float x = spriteVivo.getPosition().x;
    float y = spriteVivo.getPosition().y;
    float w = spriteVivo.getTexture()->getSize().x;
    float h = spriteVivo.getTexture()->getSize().y;
    int mouseX = position.x;
    int mouseY = position.y;

    if (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h) {
        spriteMuerto.setPosition(spriteVivo.getPosition());
        vivo = false;
        partida.scoreIncrease();
        speed.y = 4;
        speed.x = 0;
        return true;
    }
    else {
        return false;
    }
}

int Pato1::getRebotesY() const {
    return rebotesy;
}

Pato1* pato = nullptr; //se hace un apuntador para poder eliminarlo totalmente del sistema y que no se sature la memoria(lo agregue pq trababa mi laptop)

void crearPato(RenderWindow& window);
//main.cpp
int menu() {
    RenderWindow window(VideoMode(1200, 1000), "Detect Space Key Press");
    Texture rulesTexture;
    if (!rulesTexture.loadFromFile("texturas/Reglas.png")) {
        return EXIT_FAILURE;
    }

    Texture menuTexture;
    if (!menuTexture.loadFromFile("texturas/Menu.png")) {
        return EXIT_FAILURE;
    }
    Sprite rulesSprite(rulesTexture);
    rulesSprite.setScale(
        window.getSize().x / static_cast<float>(rulesTexture.getSize().x),
        window.getSize().y / static_cast<float>(rulesTexture.getSize().y)
    );

    Sprite menuSprite(menuTexture);
    menuSprite.setScale(
        window.getSize().x / static_cast<float>(menuTexture.getSize().x),
        window.getSize().y / static_cast<float>(menuTexture.getSize().y)
    );

    // Variable para controlar la visualización del sprite de reglas
    bool showRules = false;

    // Bucle principal
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Enter) {
                    window.close();
                }
                if (event.key.code == Keyboard::Space) {
                    showRules = !showRules;  // Alternar entre mostrar y no mostrar el sprite de reglas
                }
            }
        }

        window.clear();
        if (showRules) {
            window.draw(rulesSprite);
        }
        else {
            window.draw(menuSprite);
        }
        window.display();
    }

    return EXIT_SUCCESS;
}
int main() {
    menu();
    RenderWindow window(VideoMode(1200, 1000), "InfiniDuck");
    window.setFramerateLimit(120);

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("texturas/Fondo.png")) {
        return EXIT_FAILURE;
    }
    Sprite backgroundSprite(backgroundTexture);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Right && !partida.juegoSigue) {
                    partida.juegoSigue = true;
                    cout << "Inicio juego" << endl;
                    partida.reiniciarPatosVolados();
                }

                if (event.mouseButton.button == Mouse::Left && partida.juegoSigue && ron1.getBalas() > 0) {
                    ron1.disparaBala();
                    if (pato != nullptr && pato->disparoAcertado(Mouse::getPosition(window))) {
                        pato->vivo = false;
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        if (partida.juegoSigue) {
            if (pato != nullptr) {
                pato->update();
                pato->direccion();
                pato->drawTo(window);
                if (pato->getRebotesY() == 3) {
                    ron1.rondaTerminada();
                    delete pato;
                    pato = nullptr;
                    partida.patosFuera();
                    partida.incrementarPatosVolados();
                }
                else if (!pato->vivo && pato->getPositionY() + pato->tamanoY() >= 1000) {
                    ron1.rondaTerminada();
                    delete pato;
                    pato = nullptr;
                }
            }
            else if (ron1.getBalas() > 0) {
                crearPato(window);
            }
        }

        ron1.drawTo(window);
        ron1.update();
        partida.drawPatosVolados(window); // dibujar cuadros amarillos

        window.display();
    }
    return 0;
}

void crearPato(RenderWindow& window) {//crea patos y ya :D
    float x = float(100);
    float y = float(100);
    pato = new Pato1(Vector2f(x, y), window);
    pato->drawTo(window);
}
