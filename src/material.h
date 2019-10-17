namespace renderer {
    struct Colour {
        double r;
        double g;
        double b;
    };

    struct Material {
        Colour emission;
        Colour diffusion;
    };
}
