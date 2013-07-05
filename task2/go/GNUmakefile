all: check

check:
	GOPATH=$$(pwd) go test cuckoo

bench:
	GOPATH=$$(pwd) go test -bench . -benchtime 5s -cpu 1,2,4,8,16 cuckoo
