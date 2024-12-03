#include "examples.hpp"

/** \brief Jak by C++ řešilo váš úkol ze cvičení s funkcemi `qsort` a `bsearch`? */
void lambda_functions() {
    // Znáte ze cvičení! Akorát nyní už je `vector` díky šabloně typově bezpečný!
    std::vector<uts::Wiffle> values;

    // Alokuje místo pro SAMPLE_VECTOR_SIZE prvků, ale neprovede jejich konstrukci.
    // Dělat to samozřejmě nemusíme, ale vyhneme se tak realokacím při přidávání prvků.
    values.reserve(SAMPLE_VECTOR_SIZE);

    // Přidáme nějaké hodnoty do vektoru.
    values.push_back(10.0);

    // Standardní knihovna obsahuje generátor pseudonáhodných čísel.
    std::random_device rd;
    std::mt19937_64 gen{rd()};
    std::uniform_real_distribution<double> dist;

    // Generátor implementujeme pomocí tzv. lambda funkce. Na základě tohoto zápisu vytvoří kompilátor anonymní třídu,
    // která bude kromě atributů `&gen` a `&dist` obsahovat i přetížení operátoru funkčního volání -- a lambda je světě!
    // Od C++20 je možné vytvořit šablony lambda funkcí se syntaxí: `[]<typename T>() -> return_type {};`
    auto generator_lambda = [&gen, &dist]() -> double {
        return dist(gen);
    };

    // Pro ukázku je zde naznačena podoba vygenerované třídy (tzv. funktoru).
    GeneratorFunctor generator_functor{gen, dist};

    // `generator_lambda` a `generator_functor` se budou samozřejmě chovat stejně!
    std::generate_n(std::back_inserter(values), SAMPLE_VECTOR_SIZE, generator_lambda);

    // Toto jste implementovali předminulém cvičení, vzpomínáte? V C++ je implementace už typově bezpečná, tj. nemusím
    // myslet, jakým způsobem je bezpečné `const void *` přetypovat. Když bude definovaná s argumenty špatného typu, už
    // kompilace skončí chybou! A to je skvělé, protože pak nemusím mládí trávit hledáním chyby v runtime.
    auto sorter = [](const uts::Wiffle& a, const uts::Wiffle& b) {
        return a < b;
    };
    std::sort(values.begin(), values.end(), sorter);

    // Nakonec můžeme obsah vektoru vypsat.
    for (const auto &v : values) {
        std::cout << v << std::endl;
    }
}

void smart_pointers() {
    // Objekt vytvořený klasicky na zásobníku funkce `smart_pointers`.
    ps::Rectangle<uts::Wiffle> stack_rect{1.0, 1.0};
    std::cout << "Obvod obdélníku na zásobníku je " << stack_rect.perimeter() << "." << std::endl;
    std::cout << "Obsah obdélníku na zásobníku je " << stack_rect.area() << "^2." << std::endl;

    // Tohle znáte z KIV/PC (akorát trošku kamuflovaně).
    // Operátor `new` alokuje potřebnou paměť a zavolá konstruktor třídy.
    // Paměť je nutné uvolnit pomocí operátoru `delete`.
    // ! V moderním C++ se taková alokace vůbec nepoužívá (v kódu se snažíme surovým ukazatelům úplně vyhnout).
    ps::Rectangle<uts::Wiffle>* ptr_rect = new ps::Rectangle<uts::Wiffle>{2.0, 2.0};
    std::cout << "Obvod dynamicky alokovaného obdélníku je " << ptr_rect->perimeter() << "." << std::endl;
    std::cout << "Obsah dynamicky alokovaného obdélníku je " << ptr_rect->area() << "^2." << std::endl;

    {
        // Tady už je to zajímavější (příklad z knihovny Qt: https://doc.qt.io/qt-5/qscopedpointer.html)
        std::unique_ptr<ps::Rectangle<uts::Wiffle>> unique_rect = std::make_unique<ps::Rectangle<uts::Wiffle>>(3.0, 3.0);
        if (unique_rect) { // Jak je možné, že můžu udělat tohle?
            std::cout << "Objekt unique_rect byl uspesne dynamicky alokovan!" << std::endl;
        }

        std::cout << "Obvod obdelniku (unique_ptr) je " << unique_rect->perimeter() << "." << std::endl;
        std::cout << "Obsah obdelniku (unique_ptr) je " << unique_rect->area() << "^2." << std::endl;

        // Díky mechanismu přetěžování operátorů jsme schopni na úrovní jazyka zabránit vytváření kopie
        // instance třídy `std::unique_ptr`.
        // auto not_so_unique_rect = unique_rect;

        // Můžeme ovšem vlastnictví ukazatele přesunout (move semantics) -- `unique_rect` již neobsahuje platnou referenci.
        auto not_so_unique_rect = std::move(unique_rect);
    }

    // Jak funguje shared pointer: https://blog.scrt.ch/wp-content/uploads/2017/01/shared_ptr.png
    std::shared_ptr<ps::Rectangle<uts::Wiffle>> backup_ptr;
    {
        // Pokud vás už nebaví psát ty typy (kód může mnohdy nebezpečně bobtnat), tak můžete využít
        // statické automatické dedukce typu pomocí klíčového slova `auto`.
        // Zkuste se třeba podívat na to, jaký prototyp má funkce `std::move` o pár řádků výše.
        auto shared_rect = std::make_shared<ps::Rectangle<uts::Wiffle>>(4.0, 4.0);
        backup_ptr = shared_rect;
        // std::cout << shared_rect.use_count() << std::endl;

        std::cout << "Obvod obdelniku (shared_ptr) je " << shared_rect->perimeter() << "." << std::endl;
        std::cout << "Obsah obdelniku (shared_ptr) je " << shared_rect->area() << "^2." << std::endl;
    }

    std::cout << "Opustil jsem scope, ale shared pointer stale zije! ...proc?" << std::endl;

    // Je takto zdrojový kód v pořádku? Nezapomněli jsme na něco?
}

void polymorphism() {
    std::random_device rd;
    std::mt19937_64 gen{rd()};
    std::uniform_real_distribution<double> double_dist;
    std::uniform_int_distribution<int> int_dist;

    // Vytvoříme dynamicky alokované pole ukazatelů na objekty rozhraní `IPlanary`.
    // Do vektoru nemůžeme uložit objekty přímo, protože `IPlanary` je abstraktní
    // (nevíme kolik místa pro její instance alokovat -- `Circle` vs `Rectangle`).
    // Velikost instance třídy `std::unique_ptr` ovšem víme!
    std::vector<std::unique_ptr<ps::IPlanary<uts::Wiffle>>> shapes;
    shapes.reserve(SAMPLE_VECTOR_SIZE);

    // Připravíme si generátor náhodných útvarů.
    auto generator_lambda = [&gen, &double_dist, &int_dist]() -> std::unique_ptr<ps::IPlanary<uts::Wiffle>> {
        if (int_dist(gen) % 2 == 0) {
            return std::make_unique<ps::Rectangle<uts::Wiffle>>(double_dist(gen), double_dist(gen));
        }
        else {
            return std::make_unique<ps::Circle<uts::Wiffle>>(double_dist(gen));
        }
    };

    // Pole naplníme náhodně vygenerovanými obdélníky a kružnicemi.
    std::generate_n(std::back_inserter(shapes), SAMPLE_VECTOR_SIZE, generator_lambda);

    // Vypíšeme obsahy všech útvarů.
    // `shape` je konstantní reference na instanci `std::unique_ptr`. Abychom získali objekt, na který ukazuje,
    // musíme použít dereferenční operátor `*` (ano operátor je v tomto případě opět přetížený).
    for (const auto& shape : shapes) {
        std::cout << "Obsah útvaru " << *shape << " je " << shape->area() << "^2." << std::endl;
    }
}
