curl -H 'Content-Type: application/json' \
    -d '{
	"email":"maxbardi@gmail.com",
	"password":"12345"
    }' \
    -X POST \
localhost:8000/auth/login | jq '.'
