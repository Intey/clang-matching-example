float doit(int init) {
    float ret { (float)init };
    for (float i = 0.0; i < 10.0; i+=0.1) {
        ret += i;
    }
    return ret;
}
