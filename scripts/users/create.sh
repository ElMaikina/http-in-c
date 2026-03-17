curl -H 'Content-Type: application/json' \
    -d '{
	"email":"chubigod@gmail.com",
	"password":"TUXforTh3W1n"
    }' \
    -X POST \
localhost:8000/users | jq '.'
