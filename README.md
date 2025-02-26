# LinkShortener (HTTP сервис для сокращения URL)

### Тело запроса:
```json
{
  "long_url": "https://jetbrains.com",
  "short_url": "jb"
}
```

### Если запрос успешен, сервер вернет код 200 OK и JSON с коротким URL:
```json
{
  "short_url": "http://0.0.0.0:8080/jb"
}
```

### Если формат запроса неверный,вернется Bad request:
```json
{
  "error": "Invalid request format"
}
```
### Примеры запросов:
```
curl -X POST http://0.0.0.0:8080/shorten -H "Content-Type: application/json" -d '{"long_url": "https://jetbrains.com", "short_url": "jb"}' 
curl -v http://127.0.0.1:8080/jb
```

### Для запуска необходимо:
1. Клонировать репозиторий 
```
git clone https://github.com/jkrezz/LinkShortener.git
```

2. Cборать и запустить проект, либо собрать Docker-образ и запустить сервер:
```
docker build -t link_shortener
docker run -p 8080:8080 link_shortener
