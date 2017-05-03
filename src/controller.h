int l = 0;
int r = 0;

const auto W = 119;
const auto A = 97;
const auto S = 115;
const auto D = 100;

void do_contoller_rotine() {
  r = r >= 1 ? 1 : r <= -1 ? -1 : 0;
  l = l >= 1 ? 1 : l <= -1 ? -1 : 0;

  auto a = Serial.read();

  while (a != -1) {
    if (a == W) {
      l = -2;
      r = -2;
    }
    if (a == S) {
      l = 2;
      r = 2;
    }
    
    if (a == A) {
      l += 1;
      r -= 1;
    }
    if (a == D) {
      l -= 1;
      r += 1;
    }

    a = Serial.read();
  }

  wheel(R_F, R_B, r > 0 ? 255 : r < 0 ? -255 : 0);
  wheel(L_F, L_B, l > 0 ? 255 : l < 0 ? -255 : 0);
}
