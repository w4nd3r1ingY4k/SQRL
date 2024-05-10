#pragma once

#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>

#define SALT_SIZE 16  // 128 bits 16 bytes
#define PEPPER_SIZE 1 // 8 bits   1 byte
#define PRG_SIZE 16
#define RSA_KEYSIZE 3072

// Primes from https://www.rfc-editor.org/rfc/rfc5114#page-4
const CryptoPP::Integer DL_P =
    CryptoPP::Integer("0x87A8E61DB4B6663CFFBBD19C651959998CEEF608660DD0F2"
                      "5D2CEED4435E3B00E00DF8F1D61957D4FAF7DF4561B2AA30"
                      "16C3D91134096FAA3BF4296D830E9A7C209E0C6497517ABD"
                      "5A8A9D306BCF67ED91F9E6725B4758C022E0B1EF4275BF7B"
                      "6C5BFC11D45F9088B941F54EB1E59BB8BC39A0BF12307F5C"
                      "4FDB70C581B23F76B63ACAE1CAA6B7902D52526735488A0E"
                      "F13C6D9A51BFA4AB3AD8347796524D8EF6A167B5A41825D9"
                      "67E144E5140564251CCACB83E6B486F6B3CA3F7971506026"
                      "C0B857F689962856DED4010ABD0BE621C3A3960A54E710C3"
                      "75F26375D7014103A4B54330C198AF126116D2276E11715F"
                      "693877FAD7EF09CADB094AE91E1A1597");
const CryptoPP::Integer DL_G =
    CryptoPP::Integer("0x3FB32C9B73134D0B2E77506660EDBD484CA7B18F21EF2054"
                      "07F4793A1A0BA12510DBC15077BE463FFF4FED4AAC0BB555"
                      "BE3A6C1B0C6B47B1BC3773BF7E8C6F62901228F8C28CBB18"
                      "A55AE31341000A650196F931C77A57F2DDF463E5E9EC144B"
                      "777DE62AAAB8A8628AC376D282D6ED3864E67982428EBC83"
                      "1D14348F6F2F9193B5045AF2767164E1DFC967C1FB3F2E55"
                      "A4BD1BFFE83B9C80D052B985D182EA0ADB2A3B7313D3FE14"
                      "C8484B1E052588B9B7D2BBD2DF016199ECD06E1557CD0915"
                      "B3353BBB64E0EC377FD028370DF92B52C7891428CDC67EB6"
                      "184B523D1DB246C32F63078490F00EF8D647D148D4795451"
                      "5E2327CFEF98C582664B4C0F6CC41659");
const CryptoPP::Integer DL_Q = CryptoPP::Integer(
    "0x8CF83642A709A097B447997640129DA299B1A47D1EB3750BA308B0FE64F5FBD3");

const std::string B64_RSA_PRIVATE_KEY = ("MIIG/AIBADANBgkqhkiG9w0BAQEFAASCBuYwggbiAgEAAoIBgQCpWlcAs+nmvqsUSFirSSb2"
                                         "o/GBRUcZZO4msFEXDYDtMJA4v8vQXLJqwdd0rwbie5LtdgpmJLB8qYd0qbBmAJVessTZkiuV"
                                         "+uwoQM6hFDTzeC3IvK6smol6gaIxxyaUi8OOqZ3PRWB6EhW0IJhRaE2eh6vaYss7Z6JaBLw9"
                                         "k/CmiDAkxRmFPIIS+nx/9ivhd34GJjzu21JD5Ku5Tr96iyKDrJUp5ZuoXi4Chi3FQkX1WaeI"
                                         "Vm3SPnuWtiZcRg125rSXEpAd1VK0LXBvDhY5naaUAeVX11Z5HZ7p/rZ9H6fZchhGkO+m5F+q"
                                         "qbJi4ukUN9Ym4n2KhnB0DJPschWFp60T7ASNfCiV4JhMxP/VZcic3+Nia+91F/qrte8I240U"
                                         "H2z9xfTHfA6n2mtEPb4EXOijk8WWJ/LVcm8ZSkRrxilY+Uz5HlqhIfsXe8UgRXtcceq1HXue"
                                         "uKDoYX1Bl3G7LP6d9EQYvCWm/+l24Vc/jNk/mydqFIngwRhb/La9gGTH3sMCARECggGAHeLE"
                                         "Dy7O7HwAEqNa8QzowhzuYhtIyD782aak5fNS/K43kYtCFbYBXiI1FJdbkWEZ7atrPzOmrJZj"
                                         "MrSIiniS1HnmgL9xGneTFimN4DC+DNj5BU55LYSu2WIrrm5wOFTmRloq6FeJfvQh8pxXHWzC"
                                         "Z0UeU7cUzj93ANOov5KT4ScXjgSqJpI1EmhwUtEWzW9wl6xlGxelGwo8a/7Wfwl+j7ULQ6D9"
                                         "WfKAlwieqlb9SWosn5bIFgsGwD5DATmJ585b0QDR9+e4WB1dY+dZTRLAqmUGIW3KZNayfHpF"
                                         "ChPr2ybKztszIazUFwPdGngfmkEsiE4zjnw6IC/FZlHiaQUdOBf0C8AD6y4QyeC1GB7xcD1W"
                                         "0mIrMYNb+MRCYoSGQTY7XZH0bXRf1dSrqkcYJgt6JN4K3FDxBq3ELgUo3+F/Gdg8UKt0kGM3"
                                         "Jetq75kBDy9HJjS/fVvkmmkr5FaeZWUULNgzB52DfbP93ePQWoqD+uVYhIeKgy8aXInJRGMZ"
                                         "KczJAoHBAN7K9gyPxmg8WjwrgwQsOV2h9ExBvOIaxTub6xZHZsilGmSmh3ajbOF35+HiAU9d"
                                         "YqaATvJT0JTlgLeX7vhdlohiIvJ3UM3jIxqppULu3E5w0RaYbBrOvUeqdzgDNRLvGmjA9ure"
                                         "pqB72m20aDm38/ovdA2p9mUZt/h6aIp8YCfFUce7Dt/tHnXW3zgnIKgy+JCybhrivxCcb7GQ"
                                         "pFsV1t1R9RPFiHofsxBIRK32mvSiK/ar43EDZ2bmtl3/A4/eHwKBwQDCmEmUec0DSq+nbkPW"
                                         "BylwziOjfMlbl5DtA0QtvLM28StNuvEcGEDxcTrrAt9avncmpwpwyAEowwnxCiOPgnD4V3qt"
                                         "xMGTTcC6IgMJx5QnviZOUePsYN7iXf+ilAnk/QItdJTFtJ5LqW68S8zzDpXYehP9Xfv1KrTG"
                                         "sp7soU5JaqVoQmHkV/l1bt1FEKoI3bRL6tBoIn0XcRTXcUvML7VtLoMvJLQPOZfSjiCQhVJn"
                                         "5VsvE0AzCBuTXovb+n1LIt0CgcBBhv0SwOAAijioSQhqo5hm1UfaMXPKB9+oHsysMx47A2Id"
                                         "mmQT1bacqsu66B6AwR0Dy2KDghArywe9hwoM0DtVSgpHUETxQs4W5poEoJsmIS5wDrZiPM5C"
                                         "QTIfiHkFkZ5bC5PqufTFyhMRNQ+YgWXgLBMTE98OrTYb58Rk92eTOgj+gk+rRb2qMCOI/HME"
                                         "SzoMcLb42UdBHvOsshI46E5BGBrnspGNVJ4T2QUkG1rAa+7QFHASLi14njWjLOLuFCcCgcEA"
                                         "lM7O2vO65GZKFp+dSVDFVkNIbfYDZCicAIoG9cyJC+WZlcsw2T+5MRpaOz5ukK/Ehvg1KRFq"
                                         "TFjpfBbP5jaSn8ps3zwbjsL8ypJ6yz5EHmQ7aRFyDxzmrRq0bUQHkP22Xv7MHrdL34GQ6lgV"
                                         "MlZylnt4srFIM/N7LoiXpeTDZVGNi/aHGAcKDoH0gBvNUhLzSRz5uQtQmXSXd5LfnCR7rdhG"
                                         "MxwRONGwVbf6yN5sMVUJfloD6sn2+Dk9xlYjk89dAoHBAMAUhXC6iA//ii4Cl0ZTurxNvlAL"
                                         "lHA7Agvgt7uGgD7e7Nunw8yCHXR2bs4t4hFR3I8HfHusJlFq/u7+0EwPQSO5Fj3mzEB5Q+m0"
                                         "TdLMcJ0aUNSxtw8M6F5hAGGUZcOFIhQTqwhE65UGrpy2kFWkYljPx0+6+9wjX3iUV+pZNvP8"
                                         "PCDDbIfRp6i1yCVf5Il4OE1nS/Gd46LnPo3yQhJvlIWlM38KIlDgNvcANjqQARjJOsqBSg9Q"
                                         "EgbNQuCCPDc+8w==");

const std::string B64_RSA_PUBLIC_KEY = ("MIIBoDANBgkqhkiG9w0BAQEFAAOCAY0AMIIBiAKCAYEAqVpXALPp5r6rFEhYq0km9qPxgUVH"
                                        "GWTuJrBRFw2A7TCQOL/L0FyyasHXdK8G4nuS7XYKZiSwfKmHdKmwZgCVXrLE2ZIrlfrsKEDO"
                                        "oRQ083gtyLyurJqJeoGiMccmlIvDjqmdz0VgehIVtCCYUWhNnoer2mLLO2eiWgS8PZPwpogw"
                                        "JMUZhTyCEvp8f/Yr4Xd+BiY87ttSQ+SruU6/eosig6yVKeWbqF4uAoYtxUJF9VmniFZt0j57"
                                        "lrYmXEYNdua0lxKQHdVStC1wbw4WOZ2mlAHlV9dWeR2e6f62fR+n2XIYRpDvpuRfqqmyYuLp"
                                        "FDfWJuJ9ioZwdAyT7HIVhaetE+wEjXwoleCYTMT/1WXInN/jYmvvdRf6q7XvCNuNFB9s/cX0"
                                        "x3wOp9prRD2+BFzoo5PFlify1XJvGUpEa8YpWPlM+R5aoSH7F3vFIEV7XHHqtR17nrig6GF9"
                                        "QZdxuyz+nfREGLwlpv/pduFXP4zZP5snahSJ4MEYW/y2vYBkx97DAgER");