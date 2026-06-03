<<<<<<< HEAD
#include "SVGElements.hpp"
#include <sstream>


namespace svg
{
    // These must be defined!
    SVGElement::SVGElement(const std::string &transform, const Point &origin)
        : transform_(transform), transform_origin(origin) {} //3
    //contrtutor


    SVGElement::~SVGElement() {}
    //destrutor


    //3
Point SVGElement::apply_transforms(Point p) const
    {
        if (transform_.empty()) {
            return p;
        }

        size_t open_bracket = transform_.find('(');
        size_t close_bracket = transform_.find(')');
         //procura na string o parêntesis de abertura e fecho, para o transform
        
        if (open_bracket == std::string::npos || close_bracket == std::string::npos) {
            return p;
        }
        // se não encontrar retorna p, que são as cordenadas do que seria transformado

        std::string tipo = transform_.substr(0, open_bracket);
        // retira tudo o que está antes do 1 parêntesis, ou seja, o tipo de transformação que irá ocorrer
        
        std::string argumento = transform_.substr(open_bracket + 1, close_bracket - open_bracket - 1);
        // agora retira apenas os dados
        
        
        for (char &c : argumento) {
            if (c == ',') c = ' ';
        }
        // substitui as vírgulas por espaços, para que os dados sejam usados no s_tr_ing_stre_am
        
        std::stringstream ss(argumento);
        //cria um novo ss para desenvolver um novo fluxo, entre os dados

        if (tipo == "translate")
        { // se for deste tipo.... 
            int tx = 0, ty = 0;
            if (ss >> tx >> ty) { // do fluxo enunciado extrai as duas variáveis e transforma-as
                p = p.translate({tx, ty});
            }
        }
        else if (tipo == "rotate")
        {
            int graus = 0;
            if (ss >> graus) {
                p = p.rotate(transform_origin, graus);
            }
        }
        else if (tipo == "scale")
        {
            int v = 1;
            if (ss >> v) {
                p = p.scale(transform_origin, v);
            }
        }
        return p;
    }

    //3

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius,const std::string &transform, const Point &origin) //3
        : SVGElement(transform, origin), //3
        fill(fill), center(center), radius(radius)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        // 3
        Point transformed_center = apply_transforms(center);
    
        Point edge_x = { center.x + radius.x, center.y };
        Point edge_y = { center.x, center.y + radius.y };
    
        Point transformed_edge_x = apply_transforms(edge_x);
        Point transformed_edge_y = apply_transforms(edge_y);
    
        int new_rx = std::abs(transformed_edge_x.x - transformed_center.x);
        int new_ry = std::abs(transformed_edge_y.y - transformed_center.y);
    
        if (new_rx == 0) new_rx = std::abs(transformed_edge_x.y - transformed_center.y);
        if (new_ry == 0) new_ry = std::abs(transformed_edge_y.x - transformed_center.x);

        img.draw_ellipse(transformed_center, {new_rx, new_ry}, fill);
        //3
    }
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->
//escrevemos:
Circle::Circle(const Color& fill,
               const Point& center,
               const int& radius,
                const std::string &transform, const Point &origin) //3
        : SVGElement(transform, origin), //3
        fill(fill), center(center), radius(radius)
{}
void Circle::draw(PNGImage &img) const
    {
        Point transformed_center = apply_transforms(center);
        Point edge_point = { center.x + radius, center.y };
        Point transformed_edge = apply_transforms(edge_point);
        int new_radius = std::abs(transformed_edge.x - transformed_center.x);
        if (new_radius == 0) {
            new_radius = std::abs(transformed_edge.y - transformed_center.y);
        }

        img.draw_ellipse(transformed_center, {new_radius, new_radius}, fill);
    }
//escrevemos:
Polyline::Polyline(const Color& stroke,
                   const std::vector<Point>& points,
                const std::string &transform, const Point &origin) //3
        : SVGElement(transform, origin), //3
        stroke(stroke),
        points(points)
{
}

void Polyline::draw(PNGImage& img) const
{
    for(size_t i = 1; i < points.size(); i++)
    {
        img.draw_line(apply_transforms(points[i - 1]), 
                    apply_transforms(points[i]), 
                    stroke);
    }
}

//escrevemos:
Line::Line(const Color& stroke,
           const Point& p1,
           const Point& p2,
        const std::string &transform, const Point &origin) //3
        : SVGElement(transform, origin), //3
        stroke(stroke), p1(p1), p2(p2) {}
void Line::draw(PNGImage &img) const
    {
        img.draw_line(apply_transforms(p1), apply_transforms(p2), stroke);
    }
//escrevemos:
Polygon::Polygon(const Color& fill,
                 const std::vector<Point>& points, 
                const std::string &transform, const Point &origin) //3
        : SVGElement(transform, origin), //3
        fill(fill), points(points)
{
}

void Polygon::draw(PNGImage& img) const
{
    //3 - transformar o vetor
    std::vector<Point> transformed_pts;
    transformed_pts.reserve(points.size());
    for (const Point &p : points) {
        transformed_pts.push_back(apply_transforms(p));
    }
    img.draw_polygon(transformed_pts, fill);
    //3
}
//escrevemos:
Rect::Rect(const Color& fill,
           const Point& top_left,
           int width,
           int height, const std::string &transform, const Point &origin) //3
        : SVGElement(transform, origin), //3
           fill(fill), top_left(top_left), width(width), height(height) {}
void Rect::draw(PNGImage &img) const
    {
        
        std::vector<Point> pts = {
            top_left,
            {top_left.x + width - 1, top_left.y},
            {top_left.x + width - 1, top_left.y + height - 1},
            {top_left.x, top_left.y + height - 1}
        };
        //3 - transformar o vetor
        std::vector<Point> transformed_pts;
        transformed_pts.reserve(4);
        for (const Point &p : pts) {
            transformed_pts.push_back(apply_transforms(p));
        }
        img.draw_polygon(transformed_pts, fill);
        //3
    }

Group::Group() {}

Group::~Group() {
    // Como o grupo é dono dos seus elementos, temos de os apagar da memória
    for (SVGElement* elem : elements) {
        delete elem;
    }
    elements.clear();
}

void Group::addElement(SVGElement* elem) {
    if (elem != nullptr) {
        elements.push_back(elem);
    }
}

void Group::draw(PNGImage& img) const {
    // Um ciclo 'for' que percorre o vetor e desenha cada filho
    for (SVGElement* elem : elements) {
        elem->draw(img); 
        // Nota: Se a transformação do grupo tiver de afetar os filhos, 
        // a lógica de matrizes/transformações do motor tratará disso ou terá de ser aplicada aqui.
    }
}

SVGElement* Group::clone() const {
    Group* newGroup = new Group();
    // Temos de fazer uma "cópia profunda" (deep copy) de todos os elementos lá dentro
    for (SVGElement* elem : elements) {
        newGroup->addElement(elem->clone());
    }
    return newGroup;
}

// escrevemos:
Use::Use(SVGElement* elem) : clonedElement(elem) {}

Use::~Use() {
    delete clonedElement; // Apaga o elemento clonado associado a este 'use'
}

void Use::draw(PNGImage& img) const {
    if (clonedElement != nullptr) {
        clonedElement->draw(img);
    }
}

SVGElement* Use::clone() const {
    // Clona o 'use' e faz também um clone do elemento que ele aponta
    return new Use(clonedElement->clone());
}
}
=======
#include "SVGElements.hpp"
#include <sstream>
#include <cstdlib>
#include <cmath>

namespace svg
{
    // Helper function for bounds checking to prevent assertion crashes
    bool is_on_screen(const Point& p, const PNGImage& img) {
        return (p.x >= 0 && p.x < img.width() && p.y >= 0 && p.y < img.height());
    }

    // ── SVGElement base ───────────────────────────────────────────────────────
    SVGElement::SVGElement(const std::string &transform, const Point &origin)
        : transform_(transform), transform_origin(origin) {}

    SVGElement::~SVGElement() {}

    Point SVGElement::apply_transforms(Point p) const {
        if (transform_.empty()) return p;

        double cur_x = static_cast<double>(p.x);
        double cur_y = static_cast<double>(p.y);

        std::stringstream ss_full(transform_);
        std::string task;
        
        while (std::getline(ss_full, task, ')')) {
            size_t open_bracket = task.find('(');
            if (open_bracket == std::string::npos) continue;

            std::string tipo = task.substr(0, open_bracket);
            tipo.erase(0, tipo.find_first_not_of(" \t\n\r"));
            
            std::string argumento = task.substr(open_bracket + 1);
            for (char &c : argumento) if (c == ',') c = ' ';
            std::stringstream ss(argumento);

            if (tipo == "translate") {
                double tx = 0, ty = 0;
                if (ss >> tx >> ty) {
                    cur_x += tx;
                    cur_y += ty;
                }
            } else if (tipo == "rotate") {
                double graus = 0;
                if (ss >> graus) {
                    Point temp = { (int)std::round(cur_x), (int)std::round(cur_y) };
                    temp = temp.rotate(transform_origin, (int)std::round(graus));
                    cur_x = temp.x;
                    cur_y = temp.y;
                }
            } else if (tipo == "scale") {
                double v = 1;
                if (ss >> v) {
                    Point temp = { (int)std::round(cur_x), (int)std::round(cur_y) };
                    temp = temp.scale(transform_origin, (int)std::round(v));
                    cur_x = temp.x;
                    cur_y = temp.y;
                }
            }
        }
        return { (int)std::round(cur_x), (int)std::round(cur_y) };
    }

    // ── Ellipse ───────────────────────────────────────────────────────────────
    Ellipse::Ellipse(const Color &fill, const Point &center, const Point &radius,
                     const std::string &transform, const Point &origin)
        : SVGElement(transform, origin), fill(fill), center(center), radius(radius) {}

    void Ellipse::draw(PNGImage &img) const {
        Point t_center = apply_transforms(center);
        if (!is_on_screen(t_center, img)) return;

        Point edge_x = { center.x + radius.x, center.y };
        Point edge_y = { center.x, center.y + radius.y };
        Point t_edge_x = apply_transforms(edge_x);
        Point t_edge_y = apply_transforms(edge_y);

        int rx = std::abs(t_edge_x.x - t_center.x);
        int ry = std::abs(t_edge_y.y - t_center.y);
        if (rx == 0) rx = std::abs(t_edge_x.y - t_center.y);
        if (ry == 0) ry = std::abs(t_edge_y.x - t_center.x);

        img.draw_ellipse(t_center, {rx, ry}, fill);
    }

    // ── Circle ────────────────────────────────────────────────────────────────
    Circle::Circle(const Color &fill, const Point &center, const int &radius,
                   const std::string &transform, const Point &origin)
        : SVGElement(transform, origin), fill(fill), center(center), radius(radius) {}

    void Circle::draw(PNGImage &img) const {
        Point t_center = apply_transforms(center);
        if (!is_on_screen(t_center, img)) return;

        Point edge_pt = { center.x + radius, center.y };
        Point t_edge = apply_transforms(edge_pt);
        int r = std::abs(t_edge.x - t_center.x);
        if (r == 0) r = std::abs(t_edge.y - t_center.y);

        img.draw_ellipse(t_center, {r, r}, fill);
    }

    // ── Polyline ──────────────────────────────────────────────────────────────
    Polyline::Polyline(const Color &stroke, const std::vector<Point> &points,
                       const std::string &transform, const Point &origin)
        : SVGElement(transform, origin), stroke(stroke), points(points) {}

    void Polyline::draw(PNGImage &img) const {
        for (size_t i = 1; i < points.size(); i++) {
            Point p1 = apply_transforms(points[i-1]);
            Point p2 = apply_transforms(points[i]);
            if (is_on_screen(p1, img) && is_on_screen(p2, img)) {
                img.draw_line(p1, p2, stroke);
            }
        }
    }

    // ── Line ──────────────────────────────────────────────────────────────────
    Line::Line(const Color &stroke, const Point &p1, const Point &p2,
               const std::string &transform, const Point &origin)
        : SVGElement(transform, origin), stroke(stroke), p1(p1), p2(p2) {}

    void Line::draw(PNGImage &img) const {
        Point pt1 = apply_transforms(p1);
        Point pt2 = apply_transforms(p2);
        if (is_on_screen(pt1, img) && is_on_screen(pt2, img)) {
            img.draw_line(pt1, pt2, stroke);
        }
    }

    // ── Polygon ───────────────────────────────────────────────────────────────
    Polygon::Polygon(const Color &fill, const std::vector<Point> &points,
                     const std::string &transform, const Point &origin)
        : SVGElement(transform, origin), fill(fill), points(points) {}

    void Polygon::draw(PNGImage &img) const {
        std::vector<Point> t_pts;
        t_pts.reserve(points.size());
        for (const auto& p : points) {
            Point tp = apply_transforms(p);
            if (!is_on_screen(tp, img)) return; // Skip if any vertex crashes the image
            t_pts.push_back(tp);
        }
        if (!t_pts.empty()) img.draw_polygon(t_pts, fill);
    }

    // ── Rect ──────────────────────────────────────────────────────────────────
    Rect::Rect(const Color &fill, const Point &top_left, int width, int height,
               const std::string &transform, const Point &origin)
        : SVGElement(transform, origin), fill(fill), top_left(top_left), width(width), height(height) {}

    void Rect::draw(PNGImage &img) const {
    // Subtracting 1 from width and height creates the 1-pixel gap 
    // expected by many SVG rasterizers.
    std::vector<Point> pts = {
        top_left,
        {top_left.x + width - 1, top_left.y},
        {top_left.x + width - 1, top_left.y + height - 1},
        {top_left.x,             top_left.y + height - 1}
    };
    
    std::vector<Point> t_pts;
    for (const auto& p : pts) {
        Point tp = apply_transforms(p);
        if (!is_on_screen(tp, img)) return;
        t_pts.push_back(tp);
    }
    img.draw_polygon(t_pts, fill);
}

    // ── Group ─────────────────────────────────────────────────────────────────
    Group::Group(const std::vector<SVGElement*> &children,
                  const std::string &transform, const Point &origin)
        : SVGElement(transform, origin), children_(children) {}

    Group::~Group() {
        for (SVGElement* e : children_) delete e;
    }

    void Group::draw(PNGImage &img) const {
        for (SVGElement* child : children_) {
            std::string old_t = child->get_transform();
            // Parent transform is appended AFTER child transform
            child->set_transform(old_t + " " + transform_);
            child->draw(img);
            child->set_transform(old_t);
        }
    }

    SVGElement* Group::clone() const {
        std::vector<SVGElement*> cloned;
        for (SVGElement* e : children_) cloned.push_back(e->clone());
        return new Group(cloned, transform_, transform_origin);
    }

    // ── Use ───────────────────────────────────────────────────────────────────
    Use::Use(SVGElement* cloned, const std::string &transform, const Point &origin)
        : SVGElement(transform, origin), element_(cloned) {}

    Use::~Use() { delete element_; }

    void Use::draw(PNGImage &img) const {
        if (element_) {
            std::string old_t = element_->get_transform();
            element_->set_transform(old_t + " " + transform_);
            element_->draw(img);
            element_->set_transform(old_t);
        }
    }

    SVGElement* Use::clone() const {
        return new Use(element_->clone(), transform_, transform_origin);
    }
} // namespace svg
>>>>>>> 75c0479 (Fix: rotation precision, rect gap, and added .gitignore for cleaner repo)
