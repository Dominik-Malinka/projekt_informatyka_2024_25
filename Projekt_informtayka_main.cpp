#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace sf;
using namespace std;

const float szerokosc_okna = 1920;
const float wysokosc_okna = 1080;

struct Gracz {
    RectangleShape model_gracza;
    float predkosc_gracza;
    int zycia_gracza;


    Gracz(Texture& tekstura) {
        model_gracza.setSize(Vector2f(80, 80));
        model_gracza.setTexture(&tekstura);
        model_gracza.setPosition(szerokosc_okna / 2, wysokosc_okna - 140);
        predkosc_gracza = 8.0f;
        zycia_gracza = 3;
    }

    void ruch_gracza() {
        if (Keyboard::isKeyPressed(Keyboard::Left) && model_gracza.getPosition().x > 0) {
            model_gracza.move(-predkosc_gracza, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) && model_gracza.getPosition().x < szerokosc_okna - model_gracza.getSize().x) {
            model_gracza.move(predkosc_gracza, 0);
        }
    }
};

struct Pocisk {
    RectangleShape model_pocisku;
    float pr�dko��;

    Pocisk(float x, float y, float predkosc) {
        model_pocisku.setSize(Vector2f(5, 20));
        model_pocisku.setFillColor(Color::Yellow);
        model_pocisku.setPosition(x, y);
        pr�dko�� = predkosc;
    }

    void ruch_pocisku() {
        model_pocisku.move(0, pr�dko��);
    }
};

struct Kosmita {
    RectangleShape model_kosmity;
    Kosmita(float x, float y, Texture& tekstura) {
        model_kosmity.setSize(Vector2f(70, 70));
        model_kosmity.setTexture(&tekstura); // Ustawienie wsp�lnej tekstury
        model_kosmity.setPosition(x, y);
        

    }
    void ruch_kosmity(float przesuni�cieX) {
        model_kosmity.move(przesuni�cieX, 0);
    }


};

class Os�ona {
public:
    int zycia_oslony; // Pole przechowuj�ce �ycie os�ony
    RectangleShape model_oslony; // Kszta�t os�ony w grze
    bool check_collision(const FloatRect& bounding_box) const {
        return model_oslony.getGlobalBounds().intersects(bounding_box);
    }    
    

    // Konstruktor klasy Os�ona
    Os�ona(float x, float y, Texture& tekstura) {       
        zycia_oslony = 10; // Przyk�adowa warto�� pocz�tkowa
        model_oslony.setSize(Vector2f(100, 50));
        model_oslony.setTexture(&tekstura);            
        model_oslony.setPosition(x, y);
        
    }

    // Funkcja rysowania os�ony
    void rysuj(RenderWindow& okno) const {
        okno.draw(model_oslony);
    }
};

bool pomoc(Font czcionka) {
    // Tworzenie okna pomocy
    RenderWindow okno_help(VideoMode(800, 800), "Pomoc do gry Space Invaders");
    RectangleShape przyciskWyjdz(Vector2f(200, 70));

    // Tworzenie tre�ci pomocy
    Text help_tytul;
    help_tytul.setFont(czcionka);
    help_tytul.setString("Pomoc do gry Space Invaders");
    help_tytul.setCharacterSize(30);
    help_tytul.setFillColor(Color::White); // Kolor tekstu
    help_tytul.setPosition(200, 100);

    Text help_tekst;
    help_tekst.setFont(czcionka);
    help_tekst.setString("Gra Space Invaders polega na odparciu inwazji kosmitow, ktora probuje zawladnac nad twoja planeta.\n\nNa gorzekranu widnieje armia stworow, ktore mamy za zadanie zestrzelic kosmicznym statkiem.\n\nGracz moze chowac sie za oslonami, ktore gwarantuja ochrone, jednakze kazda z nich jest w stanie pochlonac tylko \n\n10 pociskow, po czym ulegaja zniszczeniu. Twoj statek ma 3 punkty zycia. Po straceniu 3 zyc lub w chwili\n\nkiedy kosmici dojda do oslon gra konczy sie porazka. Za kazdego kosmite dostajemy po 10 punktow.\n\nIlosc odpartych fal kosmitow reprezentuje statystyka poziom.\n\nStatystyki takie jak zycie, punkty, czy poziom sa wyswietlone w lewym gornym rogu gry.\n\n");
    help_tekst.setCharacterSize(15);
    help_tekst.setFillColor(Color::White);
    help_tekst.setPosition(30, 150);

    Text sterowanie_tytul;
    sterowanie_tytul.setFont(czcionka);
    sterowanie_tytul.setString("Sterowanie");
    sterowanie_tytul.setCharacterSize(30);
    sterowanie_tytul.setFillColor(Color::Red);
    sterowanie_tytul.setPosition(300, 400);

    Text help_sterowanie;
    help_sterowanie.setFont(czcionka);
    help_sterowanie.setString("-Ruch statkiem w lewo: strzalka w lewo\n\n-Ruch statkiem w prawo: strzalka w prawo\n\n-Strzal: Spacja");
    help_sterowanie.setCharacterSize(30);
    help_sterowanie.setFillColor(Color::Red);
    help_sterowanie.setPosition(30, 500);


    Text tekstNowaGra, tekstWyjdz;

    przyciskWyjdz.setFillColor(Color::Red);
    przyciskWyjdz.setPosition(300, 700);

    tekstWyjdz.setFont(czcionka);
    tekstWyjdz.setCharacterSize(30);
    tekstWyjdz.setString("WYJDZ");
    tekstWyjdz.setFillColor(Color::White);
    tekstWyjdz.setPosition(
        przyciskWyjdz.getPosition().x + (przyciskWyjdz.getSize().x - tekstWyjdz.getLocalBounds().width) / 2,
        przyciskWyjdz.getPosition().y + 10);

    Texture help_tlo;

    if (!help_tlo.loadFromFile("kosmos_help.jpg")) {

        return -1;
    }


    Sprite tlo_help;
    tlo_help.setTexture(help_tlo);


    // P�tla obs�ugi okna pomocy
    while (okno_help.isOpen()) {
        Event event;

        while (okno_help.pollEvent(event)) {
            // Zamkni�cie okna po klikni�ciu krzy�yka
            if (event.type == Event::Closed) {
                okno_help.close();
                return false; // Powr�t z funkcji, zamkni�cie okna
            }

            // Sprawdzenie klikni�cia w przycisk "WYJDZ"
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f pozycjaMyszy(static_cast<float>(Mouse::getPosition(okno_help).x),
                    static_cast<float>(Mouse::getPosition(okno_help).y));

                if (przyciskWyjdz.getGlobalBounds().contains(pozycjaMyszy)) {
                    okno_help.close();
                    return false; // Powr�t z funkcji, zamkni�cie okna
                }
            }

            // Zamkni�cie okna po naci�ni�ciu F1
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::F1) {
                okno_help.close();
                return false; // Powr�t z funkcji, zamkni�cie okna
            }

            
        }

        // Rysowanie zawarto�ci okna
        okno_help.clear(Color::Black); // Czyszczenie okna na czarno
        okno_help.draw(tlo_help);
        okno_help.draw(help_tytul);   // Rysowanie tekstu
        okno_help.draw(help_tekst);
        okno_help.draw(sterowanie_tytul);
        okno_help.draw(help_sterowanie);
        okno_help.draw(przyciskWyjdz);
        okno_help.draw(tekstWyjdz);
        okno_help.display();         // Wy�wietlenie zawarto�ci okna
    }
    return false;
}

void przegrana(RenderWindow& okno, Font& czcionka, int& punkty, Gracz& gracz, int dostep, vector<Kosmita>& kosmici, Texture& tekstura, vector<Os�ona>& oslony, Texture& tekstura_oslony, Texture& tekstura_tla) {
    kosmici.clear();
    for (int rzad = 0; rzad < 5; ++rzad) {
        for (int kolumna = 0; kolumna < 11; ++kolumna) {
            float x = 100 + kolumna * szerokosc_okna / 12.0f;
            float y = 50 + rzad * wysokosc_okna / 12.0f;
            kosmici.emplace_back(x, y, tekstura); // Przekazanie referencji do tekstury
        }
    }

    oslony.clear(); // Wyczyszczenie istniej�cych os�on
    float odleglosc_oslon = szerokosc_okna / 4.0f;
    for (int i = 1; i <= 3; ++i) {
        oslony.emplace_back(i * odleglosc_oslon - 50, wysokosc_okna - 200, tekstura_oslony); // Tworzenie i dodawanie os�ony
    }

    // Tworzenie sprite dla t�a
    Sprite game_over;
    game_over.setTexture(tekstura_tla);

    game_over.setScale(
        static_cast<float>(szerokosc_okna) / tekstura_tla.getSize().x,
        static_cast<float>(wysokosc_okna) / tekstura_tla.getSize().y
    );

    // Ustawienie tekstu ko�ca gry
    Text tekst;
    tekst.setFont(czcionka);
    tekst.setString("Przegrales! Koniec gry.");
    tekst.setCharacterSize(50);
    tekst.setFillColor(Color::White);
    tekst.setPosition(szerokosc_okna / 2 - 100, wysokosc_okna / 2 - 100);

    // Tworzenie tekstu punkt�w
    Text tekst_punktow;
    tekst_punktow.setFont(czcionka);
    tekst_punktow.setString("Punkty: " + std::to_string(punkty));
    tekst_punktow.setCharacterSize(30);
    tekst_punktow.setFillColor(Color::White);
    tekst_punktow.setPosition(szerokosc_okna / 2 - 100, wysokosc_okna / 2);

    // Tworzenie przycisk�w
    RectangleShape przyciskNowaGra(Vector2f(200, 70));
    RectangleShape przyciskWyjdz(Vector2f(200, 70));
    Text tekstNowaGra, tekstWyjdz;

    // Konfiguracja przycisk�w
    przyciskNowaGra.setFillColor(Color::Blue);
    przyciskNowaGra.setPosition(szerokosc_okna / 2 - 100, wysokosc_okna / 2 + 60);

    przyciskWyjdz.setFillColor(Color::Red);
    przyciskWyjdz.setPosition(szerokosc_okna / 2 - 100, wysokosc_okna / 2 + 150);

    // Konfiguracja tekstu dla przycisk�w
    tekstNowaGra.setFont(czcionka);
    tekstNowaGra.setCharacterSize(30);
    tekstNowaGra.setString("NOWA GRA");
    tekstNowaGra.setFillColor(Color::White);
    tekstNowaGra.setPosition(
        przyciskNowaGra.getPosition().x + (przyciskNowaGra.getSize().x - tekstNowaGra.getLocalBounds().width) / 2,
        przyciskNowaGra.getPosition().y + 10);

    tekstWyjdz.setFont(czcionka);
    tekstWyjdz.setCharacterSize(30);
    tekstWyjdz.setString("WYJDZ");
    tekstWyjdz.setFillColor(Color::White);
    tekstWyjdz.setPosition(
        przyciskWyjdz.getPosition().x + (przyciskWyjdz.getSize().x - tekstWyjdz.getLocalBounds().width) / 2,
        przyciskWyjdz.getPosition().y + 10);


    // P�tla wy�wietlania ko�cowego ekranu
    while (okno.isOpen())
    {
        Event zdarzenie;
        while (okno.pollEvent(zdarzenie))
        {
            if (zdarzenie.type == Event::Closed)
            {
                okno.close();
            }
            // Obs�uga klikni�� myszy
            if (zdarzenie.type == Event::MouseButtonPressed && zdarzenie.mouseButton.button == Mouse::Left)
            {
                Vector2f pozycjaMyszy(static_cast<float>(Mouse::getPosition(okno).x),
                    static_cast<float>(Mouse::getPosition(okno).y));

                // Sprawdzenie klikni�cia w przycisk "NOWA GRA"
                if (przyciskNowaGra.getGlobalBounds().contains(pozycjaMyszy))
                {
                    // Zresetuj gr� tutaj
                    punkty = 0; // Resetowanie punkt�w
                    gracz.zycia_gracza = 3;
                    dostep = 1;
                    return;
                }
                // Sprawdzenie klikni�cia w przycisk "WYJDZ"
                if (przyciskWyjdz.getGlobalBounds().contains(pozycjaMyszy))
                {
                    dostep = 0;
                    okno.close();
                }
            }
        }

        // Rysowanie element�w
        okno.clear();
        okno.draw(game_over);
        okno.draw(tekst);
        okno.draw(tekst_punktow);
        okno.draw(przyciskNowaGra);
        okno.draw(przyciskWyjdz);
        okno.draw(tekstNowaGra);
        okno.draw(tekstWyjdz);
        okno.display();
    }
}

bool potwierdzenieWyjscia(RenderWindow& okno,Font& czcionka) {
    // Utworzenie okna potwierdzenia wyj�cia
    RenderWindow oknoWyjscia(VideoMode(300, 150), "WYJSCIE");

   
    Text pytanie;
    pytanie.setString("Czy na pewno chcesz wyjsc?");
    pytanie.setFont(czcionka);
    pytanie.setCharacterSize(20);
    pytanie.setFillColor(Color::White);
    pytanie.setPosition(5, 30);

    RectangleShape przyciskTak(Vector2f(100, 40));
    przyciskTak.setFillColor(Color::Green);
    przyciskTak.setPosition(40, 80);

    Text tekstTak("TAK", czcionka, 16);
    tekstTak.setFillColor(Color::Black);
    tekstTak.setPosition(70, 90);

    RectangleShape przyciskNie(Vector2f(100, 40));
    przyciskNie.setFillColor(Color::Red);
    przyciskNie.setPosition(180, 80);

    Text tekstNie("NIE", czcionka, 16);
    tekstNie.setFillColor(Color::Black);
    tekstNie.setPosition(220, 90);

    // Obs�uga okna potwierdzenia
    while (oknoWyjscia.isOpen()) {
        Event event;

        while (oknoWyjscia.pollEvent(event)) {
            if (event.type == Event::Closed) {
                oknoWyjscia.close();
                return false; // Domy�lna akcja - kontynuowanie programu
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f pozycjaMyszy(static_cast<float>(Mouse::getPosition(oknoWyjscia).x),
                    static_cast<float>(Mouse::getPosition(oknoWyjscia).y));

                // Klikni�cie "TAK" - zako�czenie programu
                if (przyciskTak.getGlobalBounds().contains(pozycjaMyszy)) {
                    oknoWyjscia.close();
                    okno.close();                   
                    return true; // Potwierdzenie wyj�cia
                    return 0;
                }

                // Klikni�cie "NIE" - powr�t do programu
                if (przyciskNie.getGlobalBounds().contains(pozycjaMyszy)) {
                    oknoWyjscia.close();
                    return false; // Kontynuowanie programu
                }
            }
        }

        // Rysowanie element�w okna
        oknoWyjscia.clear(Color::Black);
        oknoWyjscia.draw(pytanie);
        oknoWyjscia.draw(przyciskTak);
        oknoWyjscia.draw(tekstTak);
        oknoWyjscia.draw(przyciskNie);
        oknoWyjscia.draw(tekstNie);
        oknoWyjscia.display();
    }

    return false; // W przypadku zamkni�cia okna kontynuujemy program
}

int main()
{
    int dostep = 0;
    srand(static_cast<unsigned>(time(nullptr)));
    int punkty = 0;
    int poziom = 1;

    Font czcionka;
    czcionka.loadFromFile("arial1.ttf");

    Text tekst1;
    tekst1.setFont(czcionka);
    tekst1.setString("WITAM W GRZE SPACE INVADERS!!!");
    tekst1.setCharacterSize(50);
    tekst1.setFillColor(Color::White);
    tekst1.setPosition(szerokosc_okna / 2 - 400, wysokosc_okna / 2 - 500);

    // Tworzenie przycisk�w
    RectangleShape przyciskNowaGra(Vector2f(200, 70));
    RectangleShape cien_tytul(Vector2f(1920, 1080));
    cien_tytul.setFillColor(Color(0, 0, 0, 80));
    cien_tytul.setPosition(0,0);
    RectangleShape przyciskWyjdz(Vector2f(200, 70));
    Text tekstNowaGra, tekstWyjdz;

    // Konfiguracja przycisk�w
    przyciskNowaGra.setFillColor(Color(0,0,255,200));
    przyciskNowaGra.setPosition(szerokosc_okna / 2 - 100, wysokosc_okna / 2 - 100);

    przyciskWyjdz.setFillColor(Color(255,0,0,200));
    przyciskWyjdz.setPosition(szerokosc_okna / 2 - 100, wysokosc_okna / 2);

    // Konfiguracja tekstu dla przycisk�w
    tekstNowaGra.setFont(czcionka);
    tekstNowaGra.setCharacterSize(30);
    tekstNowaGra.setString("NOWA GRA");
    tekstNowaGra.setFillColor(Color::White);
    tekstNowaGra.setPosition(
        przyciskNowaGra.getPosition().x + (przyciskNowaGra.getSize().x - tekstNowaGra.getLocalBounds().width) / 2,
        przyciskNowaGra.getPosition().y + 10);

    tekstWyjdz.setFont(czcionka);
    tekstWyjdz.setCharacterSize(30);
    tekstWyjdz.setString("WYJDZ");
    tekstWyjdz.setFillColor(Color::White);
    tekstWyjdz.setPosition(
        przyciskWyjdz.getPosition().x + (przyciskWyjdz.getSize().x - tekstWyjdz.getLocalBounds().width) / 2,
        przyciskWyjdz.getPosition().y + 10);

    RenderWindow okno1(VideoMode(1920, 1080), "MENU GLOWNE");

    Texture glowne_tlo;

    if (!glowne_tlo.loadFromFile("glowne_tlo.png")) {

        return -1;
    }

    Texture x_wing;

    if (!x_wing.loadFromFile("x_wing1.png")) {

        return -1;
    }

    Sprite tlo_glowne;
    tlo_glowne.setTexture(glowne_tlo);
   
    ConvexShape pieciokat;   
    pieciokat.setPointCount(5);
    pieciokat.setTexture(&x_wing);
    pieciokat.setPoint(0, { 100.0f, 0.0f });    // Wierzcho�ek g�rny
    pieciokat.setPoint(1, { 195.1f, 69.1f });  // Prawy g�rny
    pieciokat.setPoint(2, { 158.8f, 180.9f }); // Prawy dolny
    pieciokat.setPoint(3, { 41.2f, 180.9f });  // Lewy dolny
    pieciokat.setPoint(4, { 4.9f, 69.1f });    // Lewy g�rny
    
    


    // Ustawienie pozycji kszta�tu w �rodku okna
    pieciokat.setOrigin(100.f, 100.f); // �rodek masy kszta�tu
    pieciokat.setPosition(1700,900);

    float rotationSpeed = 90.f; // Stopnie na sekund�

    // G��wna p�tla aplikacji
    sf::Clock clock;
    
    while (okno1.isOpen())
    {
        // Obs�uga zdarze�
        Event zdarzenie;
        while (okno1.pollEvent(zdarzenie))
        {
            if (zdarzenie.type == Event::Closed)
            {
                okno1.close();
            }
            if (Keyboard::isKeyPressed(Keyboard::F1)) {
                pomoc(czcionka);
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                potwierdzenieWyjscia(okno1, czcionka);
            }

            // Obs�uga klikni�� myszy
            if (zdarzenie.type == Event::MouseButtonPressed && zdarzenie.mouseButton.button == Mouse::Left)
            {
                Vector2f pozycjaMyszy(static_cast<float>(Mouse::getPosition(okno1).x),
                    static_cast<float>(Mouse::getPosition(okno1).y));

                if (przyciskNowaGra.getGlobalBounds().contains(pozycjaMyszy))
                {
                    dostep = 1;
                    okno1.close();
                }
                if (przyciskWyjdz.getGlobalBounds().contains(pozycjaMyszy))
                {
                    dostep = 0;
                    okno1.close();
                }
            }
        }

        // Logika gry/animacji
        float deltaTime = clock.restart().asSeconds();
        pieciokat.rotate(rotationSpeed * deltaTime);

        // Renderowanie
        okno1.clear();
        okno1.draw(tlo_glowne);
        okno1.draw(pieciokat);
        okno1.draw(cien_tytul);
        okno1.draw(tekst1);
        okno1.draw(przyciskNowaGra);
        okno1.draw(przyciskWyjdz);
        okno1.draw(tekstNowaGra);
        okno1.draw(tekstWyjdz);
        okno1.display();
    }




   

    while (dostep == 1)
    {
        RenderWindow okno(VideoMode(szerokosc_okna, wysokosc_okna), "Space Invaders");
        okno.setFramerateLimit(60);
        Texture statek;
        if (!statek.loadFromFile("statek123.png")) {
            return -1; // Obs�uga b��du
        }
        Gracz gracz(statek);

        vector<Pocisk> pociski;
        vector<Pocisk> pociski_kosmitow;

        Texture tekstura_kosmity;
        if (!tekstura_kosmity.loadFromFile("misiek1.png")) {
            return -1; // Obs�uga b��du
        }

        Texture tekstura_oslony;
        if (!tekstura_oslony.loadFromFile("oslona.png")) {
            return -1; // Obs�uga b��du
        }

        Texture teksturaTla;


        if (!teksturaTla.loadFromFile("goatedtlo.png")) {
           
            return -1;
        }

        Texture tlo_przegrana;


            if (!tlo_przegrana.loadFromFile("tlo_przegrana.png")) {

                return -1;
            }

        // Tworzenie sprite dla t�a
        Sprite tlo;
        tlo.setTexture(teksturaTla);

        
        vector<Kosmita> kosmici;
        for (int rzad = 0; rzad < 5; ++rzad) {
            for (int kolumna = 0; kolumna < 11; ++kolumna) {
                float x = 100 + kolumna * szerokosc_okna / 14.0f;
                float y = 50 + rzad * wysokosc_okna / 12.0f;
                kosmici.emplace_back(x, y, tekstura_kosmity); // Przekazanie referencji do tekstury
            }
        }


        vector<Os�ona> oslony;
        float odleglosc_oslon = szerokosc_okna / 4.0f;
        for (int i = 1; i <= 3; ++i) {
            float x = i * odleglosc_oslon - 50;
            float y = wysokosc_okna - 200;
            oslony.emplace_back(x, y, tekstura_oslony);
        }

        float kierunekKosmit�w = 1.0f;
        Clock zegarRuchuKosmit�w;
        Clock zegarStrza�u;
        Clock zegarStrza�uKosmit�w;
       

        Text tekst;
        tekst.setFont(czcionka);
        tekst.setCharacterSize(24);
        tekst.setFillColor(Color::White);

        Text tekst_punktow;
        tekst_punktow.setFont(czcionka);
        tekst_punktow.setCharacterSize(24);
        tekst_punktow.setFillColor(Color::White);

        Text tekst_lvl;
        tekst_lvl.setFont(czcionka);
        tekst_lvl.setCharacterSize(24);
        tekst_lvl.setFillColor(Color::White);



        while (okno.isOpen()) {
            Event zdarzenie;
            while (okno.pollEvent(zdarzenie)) {
                if (zdarzenie.type == Event::Closed) {
                    okno.close();
                }
            }
            gracz.ruch_gracza();
            for (auto& pocisk : pociski_kosmitow) {

                // Kolizja z graczem
                if (pocisk.model_pocisku.getGlobalBounds().intersects(gracz.model_gracza.getGlobalBounds()))
                {
                    gracz.zycia_gracza--; // Gracz traci �ycie
                    pocisk.model_pocisku.setPosition(-100, -100); // Usuwamy pocisk

                    if (gracz.zycia_gracza <= 0)
                    {
                        for (auto& oslona : oslony) {
                            przegrana(okno, czcionka, punkty, gracz, dostep, kosmici, tekstura_kosmity, oslony, tekstura_oslony, tlo_przegrana);

                            if (dostep == 0) {
                                okno.close();
                            }
                            break;


                        }
                       
                    }
                }
                for (const auto& kosmita : kosmici)
                {
                    for (auto& oslona : oslony)
                    {
                        if (kosmita.model_kosmity.getGlobalBounds().intersects(oslona.model_oslony.getGlobalBounds()))
                        {                         
                            przegrana(okno, czcionka, punkty, gracz, dostep, kosmici, tekstura_kosmity, oslony, tekstura_oslony, tlo_przegrana);
                        }
                    }
                }
                // Kolizja z os�onami
                for (auto& oslona : oslony) {
                    if (oslona.check_collision(pocisk.model_pocisku.getGlobalBounds())) {
                        oslona.zycia_oslony--; // Os�ona traci punkt �ycia
                        pocisk.model_pocisku.setPosition(-100, -100); // Usuwamy pocisk
                        if (oslona.zycia_oslony <= 0) {
                            oslona.model_oslony.setPosition(-100, -100); // Usuwamy os�on�
                        }
                    }
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {               
                potwierdzenieWyjscia(okno, czcionka);
            }

            for (auto& pocisk : pociski) {
                // Kolizja pocisku gracza z os�onami
                for (auto& oslona : oslony) {
                    if (oslona.check_collision(pocisk.model_pocisku.getGlobalBounds())) {
                        oslona.zycia_oslony--;
                        pocisk.model_pocisku.setPosition(-100, -100); // Usuwamy pocisk
                        if (oslona.zycia_oslony <= 0) {
                            oslona.model_oslony.setPosition(-100, -100); // Usuwamy os�on�
                        }
                    }
                }
            }
            float szybkosc_strzalu_gracza = zegarStrza�u.getElapsedTime().asSeconds();
            if (Keyboard::isKeyPressed(Keyboard::Space) && szybkosc_strzalu_gracza > 0.5) {
                pociski.emplace_back(gracz.model_gracza.getPosition().x + gracz.model_gracza.getSize().x / 2 - 2.5f, gracz.model_gracza.getPosition().y, -15.0f);
                zegarStrza�u.restart();
            }
            float szybkosc_strzalu_kosmity = zegarStrza�uKosmit�w.getElapsedTime().asSeconds();
            if (szybkosc_strzalu_kosmity > 0.75) {
                if (!kosmici.empty()) {
                    int losowyKosmita = rand() % kosmici.size();
                    Vector2f pozycja = kosmici[losowyKosmita].model_kosmity.getPosition();
                    pociski_kosmitow.emplace_back(pozycja.x + 25, pozycja.y + 50, 10.0f);
                }
                zegarStrza�uKosmit�w.restart();
            }

            for (auto& pocisk : pociski) {
                pocisk.ruch_pocisku();
            }
            for (auto& pocisk : pociski_kosmitow) {
                pocisk.ruch_pocisku();
            }
            pociski.erase(remove_if(pociski.begin(), pociski.end(),
                [](const Pocisk& p) { return p.model_pocisku.getPosition().y < 0; }),
                pociski.end());
            pociski_kosmitow.erase(remove_if(pociski_kosmitow.begin(), pociski_kosmitow.end(),
                [](const Pocisk& p) { return p.model_pocisku.getPosition().y > wysokosc_okna; }),
                pociski_kosmitow.end());

            float szybkosc_ruchu_kosmitow = zegarRuchuKosmit�w.getElapsedTime().asMilliseconds();

            if (szybkosc_ruchu_kosmitow > 700) {
                float przesuni�cieX = 20.0f * kierunekKosmit�w;

                bool zmianaKierunku = false;
                for (const auto& kosmita : kosmici) {
                    if ((kosmita.model_kosmity.getPosition().x + przesuni�cieX > szerokosc_okna - 50) ||
                        (kosmita.model_kosmity.getPosition().x + przesuni�cieX < 0)) {
                        kierunekKosmit�w *= -1.0f;
                        zmianaKierunku = true;
                        break;
                    }
                }

                for (auto& kosmita : kosmici) {
                    kosmita.ruch_kosmity(zmianaKierunku ? 0 : przesuni�cieX);
                    if (zmianaKierunku) {
                        kosmita.model_kosmity.move(0, 30);
                    }
                }

                zegarRuchuKosmit�w.restart();
            }


            for (auto& pocisk : pociski) {
                for (auto& kosmita : kosmici) {
                    if (pocisk.model_pocisku.getGlobalBounds().intersects(kosmita.model_kosmity.getGlobalBounds())) {
                        punkty += 10;
                        kosmita.model_kosmity.setPosition(-100, -100);
                        pocisk.model_pocisku.setPosition(-100, -100);

                    }
                }
            }

           
            
                if (punkty == 550 * poziom)
                {
                    for (int rzad = 0; rzad < 5; ++rzad) {
                        for (int kolumna = 0; kolumna < 11; ++kolumna) {
                            float x = 100 + kolumna * szerokosc_okna / 14.0f;
                            float y = 50 + rzad * wysokosc_okna / 12.0f;
                            kosmici.emplace_back(x, y, tekstura_kosmity); // Przekazanie referencji do tekstury
                        }
                        
                    }
                    poziom += 1;
                }
            
            kosmici.erase(remove_if(kosmici.begin(), kosmici.end(),
                [](const Kosmita& k) { return k.model_kosmity.getPosition().x < -50; }),
                kosmici.end());

            tekst.setString("Zycia: " + to_string(gracz.zycia_gracza));
            tekst.setPosition(10, 10);
            tekst_punktow.setString("Punkty: " + to_string(punkty));
            tekst_punktow.setPosition(10, 40);
            tekst_lvl.setString("Poziom: " + to_string(poziom));
            tekst_lvl.setPosition(10, 70);

            okno.clear();
            okno.draw(tlo);
            okno.draw(gracz.model_gracza);

            for (const auto& pocisk : pociski) {
                okno.draw(pocisk.model_pocisku);
            }
            for (const auto& pocisk : pociski_kosmitow) {
                okno.draw(pocisk.model_pocisku);
            }
            for (const auto& kosmita : kosmici) {
                okno.draw(kosmita.model_kosmity);

            }
            for (const auto& oslona : oslony) {
                oslona.rysuj(okno);
            }
            okno.draw(tekst);
            okno.draw(tekst_punktow);
            okno.draw(tekst_lvl);
            okno.display();


        }

        return 0;
    }
}

