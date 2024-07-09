// Praktika_var_5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
//
//CONSTANTS
//
static const float pi = 3.141592654f;
static const int screenHeight = 1200;
static const int screenWidth = 1600;




struct Color {
    int red;
    int green;
    int blue;
};

struct Point {
    int x;
    int y;
};


class Figure {
protected:
    double width;
    double height;
    Point position;
    Color color;
public:
    virtual void moveRight(int distance) = 0;
    virtual void moveLeft(int distance) = 0;
    virtual void moveUp(int distance) = 0;
    virtual void moveDown(int distance) = 0;
    virtual void increaseWidth(int points) = 0;
    virtual void increaseHeight(int points) = 0;
    virtual void reduceWidth(int points) = 0;
    virtual void reduceHeight(int points) = 0;
    virtual void setColor(Color color) = 0;
    int getWidth() const {
        return width;
    }
    int getHeight() const{
        return height;
    }
    Color getColor() const{
        return color;
    }
    Point getPosition() const{
        return position;
    }

};


class EllipseModel : public Figure {
public:
    EllipseModel() {
        width = 0;
        height = 0;
        position = Point{0, 0};
        color = Color{ 0,0,0 };
    }
    
    EllipseModel(int width, int height, Point position, Color color) {
        this->width = width;
        this->height = height;
        this->position = position;
        this->color = color;
    }
    void moveRight(int distance) override{
        position.x+=distance;
    }
    void moveLeft(int distance) override{
        position.x-=distance;
    }
    void moveUp(int distance) override {
        position.y -= distance;
    }
    void moveDown(int distance) override {
        position.y += distance;
    }
    void printInfo() {
        std::cout << "Position: " << position.x << "," << position.y << std::endl;
        std::cout << "Width: " << width << std::endl;
        std::cout << "Height: " << height << std::endl;
        std::cout << "Color: " << color.red << "," << color.green << "," << color.blue << std::endl;
    }
    void increaseWidth(int points) override {
        width += points;
    };
    void increaseHeight(int points) override {
        height += points;
    }
    void reduceWidth(int points) override {
        width -= points;
    };
    void reduceHeight(int points) override {
        height -= points;
    }
    void setColor(Color color) override {
        this->color = color;
    }
    void setWidth(int width) {
        this->width = width;
    }
    void setHeight(int height) {
        this->height = height;
    }
    
   
    int getUpperBound() const{
        return position.y + height / 2;
    }
    int getLowerBound() const{
        return position.y - height / 2;
    }
    int getRightBound() const{
        return position.x + width / 2;
    }
    int getLeftBound() const{
        return position.x - width / 2;
    }
    
   
};





class ViewModel {
    /// <summary>
    /// constants
    /// </summary>
    const std::vector<Color> rainbowColors = {
        {148,0,211},
        {75,0,130},
        {0,0,255},
        {0,255,0},
        {255,255,0},
        {255,127,0},
        {255,0,0}
    };
    static const int ellipseQuality = 90;
    static const int sizeStep = 15;
    static const int moveStep = 10;
    static const int minEllipseSize = 50;
    static const int defaultEllipseSize = 150;

    //Non-constant fields

    std::vector<EllipseModel> ellipses;
    int selectedEllipseIndex;
    int isFluctuatingActive;
    int currentRainbowColorIndex;
    
    //
    //Create ellipse shape from ellipse model
    //

    sf::ConvexShape createEllipseShape(EllipseModel const &ellipse) {
        sf::ConvexShape ellipseShape;
        ellipseShape.setPointCount(ellipseQuality);
        for (unsigned short i = 0; i < ellipseQuality; ++i) {
            float rad = (360 / ellipseQuality * i) / (360 / pi / 2);
            float x = cos(rad) * ellipse.getWidth()/2.0;
            float y = sin(rad) * ellipse.getHeight()/2.0;

            ellipseShape.setPoint(i, sf::Vector2f(x, y));
        };
        Point pos = ellipse.getPosition();
        ellipseShape.setPosition(pos.x, pos.y);
        return ellipseShape;
    }



    bool checkIntersectionWithPoint(EllipseModel const& ellipse, Point const& point) {
        Point ellipsePos = ellipse.getPosition();
        int ellipseHeight = ellipse.getHeight();
        int ellipseWidth = ellipse.getWidth();
        if (point.y<(ellipsePos.y + ellipseHeight / 2) &&
            point.y>(ellipsePos.y - ellipseHeight / 2) &&
            point.x > (ellipsePos.x - ellipseWidth / 2) &&
            point.x < (ellipsePos.x + ellipseWidth / 2)) return true;
        return false;
    }

    //
    //Creating if less then 10
    //

    void createEllipse(Point position) {
        auto newEllipse = EllipseModel(defaultEllipseSize, defaultEllipseSize, position, rainbowColors[rand()%rainbowColors.size()]);
        newEllipse.printInfo();
        if (checkIntersections(newEllipse))return;
        if (newEllipse.getLeftBound() < 0 || newEllipse.getRightBound() > screenWidth
            || newEllipse.getUpperBound() > screenHeight
            || newEllipse.getLowerBound() < 0)return;
        newEllipse.setHeight(rand()%(defaultEllipseSize-minEllipseSize)+minEllipseSize);
        newEllipse.setWidth(rand()%(defaultEllipseSize-minEllipseSize)+minEllipseSize);
        if (ellipses.size() < 10) {
            ellipses.push_back(newEllipse);
        }
    }
    //
    //Intersection between diven ellipse and other ellipses
    //

    bool checkIntersections(EllipseModel& const ellipse) {
        for (auto& const otherEllipse: ellipses) {
            //Same ellipse
            if (otherEllipse.getPosition().x == ellipse.getPosition().x &&
                otherEllipse.getPosition().y == ellipse.getPosition().y) continue;
            //approximating
            if (ellipse.getLeftBound() <= otherEllipse.getRightBound() && ellipse.getRightBound() >= otherEllipse.getLeftBound() &&
                ellipse.getUpperBound() >= otherEllipse.getLowerBound() && ellipse.getLowerBound() <=  otherEllipse.getUpperBound()) {
                return true;
            }
            
        }
        return false;
    }

    //RGB to index in vector of rainbow colors

    int colorToRainbowColorsIndex(Color color) {
        for (int i = 0; i < rainbowColors.size(); i++) {
            if (color.red == rainbowColors[i].red
                && color.green == rainbowColors[i].blue
                && color.blue == rainbowColors[i].blue)
                return i;
        }
        return -1;
    }


public:
    ViewModel() {
        selectedEllipseIndex = 0;
        isFluctuatingActive = 0;
        currentRainbowColorIndex = -1;
    }
    //Transform models to shapes for the view
    std::vector<sf::ConvexShape> getEllipseShapes() {
        std::vector<sf::ConvexShape> ellipseShapes;
        for (int i = 0; i < ellipses.size(); i++) {
            auto ellipseShapse = createEllipseShape(ellipses[i]);
            if (i == selectedEllipseIndex) {
                ellipseShapse.setOutlineColor(sf::Color::Black);
                ellipseShapse.setOutlineThickness(5);
            }
            ellipseShapse.setFillColor(sf::Color(ellipses[i].getColor().red, ellipses[i].getColor().green, ellipses[i].getColor().blue));
            ellipseShapes.push_back(ellipseShapse);
        }
        return ellipseShapes;
    }


    //Get selected ellipse position for the view
    Point getSelectedEllipsePosition() {
        return ellipses[selectedEllipseIndex].getPosition();
    }
    //Get fluctuating status
    bool getFluctuationStatus() {
        return (bool)isFluctuatingActive;
    }

    //if click on ellipse, select, else try creating a new one
    
    void handleClick(Point position) {
        for (int i = 0; i < ellipses.size();i++) {
            if (checkIntersectionWithPoint(ellipses[i], position)) {
                isFluctuatingActive = 0;
                selectedEllipseIndex = i;
                currentRainbowColorIndex = colorToRainbowColorsIndex(ellipses[i].getColor());
                return;
            }
        }
        createEllipse(position);
        selectedEllipseIndex = -1;
    }

 

    //
    //Delete selected ellipse
    //
    void deleteEllipse() {
        if (selectedEllipseIndex == -1) return;
        ellipses.erase(ellipses.begin() + selectedEllipseIndex);
        selectedEllipseIndex = -1;
        
    }

    //
    //Change size 
    //
    //width
    void increaseWidth() {
        if (selectedEllipseIndex == -1) return;
        //screen borders
        if (ellipses[selectedEllipseIndex].getLeftBound() - sizeStep<0 ||
            ellipses[selectedEllipseIndex].getRightBound() + sizeStep>screenWidth)return;
        ellipses[selectedEllipseIndex].increaseWidth(sizeStep);
        
        if (checkIntersections(ellipses[selectedEllipseIndex])) {
            ellipses[selectedEllipseIndex].reduceWidth(sizeStep);
        }
    }
    void reduceWidth() {
        if (selectedEllipseIndex == -1 || ellipses[selectedEllipseIndex].getWidth() < sizeStep*2) return;
        ellipses[selectedEllipseIndex].reduceWidth(sizeStep);
            
    }
    //height
    void increaseHeight() {
        if (selectedEllipseIndex == -1) return;
        //screen borders
        if (ellipses[selectedEllipseIndex].getUpperBound() + sizeStep >screenHeight ||
            ellipses[selectedEllipseIndex].getLowerBound() - sizeStep<0)return;
        ellipses[selectedEllipseIndex].increaseHeight(sizeStep);

        if (checkIntersections(ellipses[selectedEllipseIndex])) {
            ellipses[selectedEllipseIndex].reduceHeight(sizeStep);
        }
    }
    void reduceHeight() {
        if (selectedEllipseIndex == -1 || ellipses[selectedEllipseIndex].getHeight() < sizeStep*2) return;
        ellipses[selectedEllipseIndex].reduceHeight(sizeStep);

    }


    //Movement
    int moveRight() {
        if (selectedEllipseIndex == -1) return 1;
        if (ellipses[selectedEllipseIndex].getRightBound()+moveStep > screenWidth)return 1;
        ellipses[selectedEllipseIndex].moveRight(moveStep);
        if (checkIntersections(ellipses[selectedEllipseIndex])) {
            ellipses[selectedEllipseIndex].moveLeft(moveStep);
            return 1;
        }
        return 0;
    }
    int moveLeft() {
        if (selectedEllipseIndex == -1) return 1;
        if (ellipses[selectedEllipseIndex].getLeftBound()-moveStep < 0)return 1;
        ellipses[selectedEllipseIndex].moveLeft(moveStep);
        if (checkIntersections(ellipses[selectedEllipseIndex])) {
            ellipses[selectedEllipseIndex].moveRight(moveStep);
            return 1;
        }
        return 0;
    }
    int moveUp(){
        if (selectedEllipseIndex == -1) return 1;
        if (ellipses[selectedEllipseIndex].getLowerBound() - moveStep < 0)return 1;
        ellipses[selectedEllipseIndex].moveUp(moveStep);
        if (checkIntersections(ellipses[selectedEllipseIndex])) {
            ellipses[selectedEllipseIndex].moveDown(moveStep);
            return 1;
        }
        return 0;
    }
    int moveDown() {
        if (selectedEllipseIndex == -1) return 1;
        if (ellipses[selectedEllipseIndex].getUpperBound() + moveStep > screenHeight)return 1;
        ellipses[selectedEllipseIndex].moveDown(moveStep);
        if (checkIntersections(ellipses[selectedEllipseIndex])) {
            ellipses[selectedEllipseIndex].moveUp(moveStep);
            return 1;
        }
        return 0;
    }

    //
    //Fluctuation
    //
    //Turn fluctuation on and off
    void fluctuationSwitch() {
        if (selectedEllipseIndex == -1)return;
        if (isFluctuatingActive) {
            isFluctuatingActive = 0;
        }
        else {
            isFluctuatingActive = 1;
        }
    }
    //Fluctuation itself
    void fluctuate() {
        if (isFluctuatingActive == 1) {
            if (moveUp())isFluctuatingActive = -1;
        }
        else {
            if (moveDown())isFluctuatingActive = 1;
        }
    }

    //
    //Color changing
    //
    void changeColor() {
        if (selectedEllipseIndex == -1)return;
        currentRainbowColorIndex++;
        if (currentRainbowColorIndex == rainbowColors.size())currentRainbowColorIndex = 0;
        ellipses[selectedEllipseIndex].setColor(rainbowColors[currentRainbowColorIndex]);
    }

};



class View {
    ViewModel vm;
    bool isFluctuationActive;
public:
    void startCycle() {
        sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Ellipses");
        
        while (window.isOpen())
        {
            isFluctuationActive = vm.getFluctuationStatus();
            if (isFluctuationActive)vm.fluctuate();
            auto ellipsesShapes = vm.getEllipseShapes();
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::MouseButtonPressed) {
                    vm.handleClick({ sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y });
                }
               
                //delete
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    vm.deleteEllipse();
                }
                //change size
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    vm.increaseWidth();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    vm.reduceWidth();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    vm.increaseHeight();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    vm.reduceHeight();
                }
                //movement
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    vm.moveUp();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    vm.moveDown();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    vm.moveLeft();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    vm.moveRight();
                }
                if (event.type == sf::Event::KeyReleased) {
                    //fluctuation
                    if (event.key.code == sf::Keyboard::F) {
                        vm.fluctuationSwitch();
                    }
                    //color
                    if (event.key.code == sf::Keyboard::C) {
                        vm.changeColor();
                    }
                }
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.clear(sf::Color::White);
            for (auto ellipse : ellipsesShapes) {
                window.draw(ellipse);
            }
      
            window.display();
        }
    }
};







int main()
{
    View view;
    view.startCycle();

    

    return 0;
}