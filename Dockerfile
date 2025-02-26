FROM fedora:latest

LABEL authors="jkrez"

# Устанавливаем необходимые пакеты
RUN dnf update -y && dnf install -y \
    gcc-c++ make wget cmake\
    boost-devel \
    nlohmann-json-devel \
    asio-devel \
    cmake \
    && dnf clean all

WORKDIR /app

# Копируем файлы проекта
COPY . .

RUN mkdir -p build
WORKDIR /app/build

# Запускаем CMake и сборку проекта
RUN cmake .. && make -j$(nproc)

EXPOSE 8080

CMD ["./link_shortener"]
