FROM alpine:3.9 as esy

ENV TERM=dumb LD_LIBRARY_PATH=/usr/local/lib:/usr/lib:/lib

ARG DB_CONNECTION=postgresql://postgres:postgres@localhost:5432/community_dev
ENV DB_CONNECTION ${DB_CONNECTION}

RUN apk add --no-cache ca-certificates wget bash curl perl-utils git patch gcc g++ musl-dev make m4 gmp-dev linux-headers openssl-dev postgresql-libs postgresql-dev nodejs nodejs-npm

RUN wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub
RUN wget https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.28-r0/glibc-2.28-r0.apk
RUN apk add --no-cache glibc-2.28-r0.apk

RUN npm install -g --unsafe-perm esy@latest

RUN mkdir /app
WORKDIR /app

COPY . .

RUN esy install
RUN esy build

RUN esy test
RUN esy e2e

RUN esy bash -c 'cp $cur__target_dir/default/executable/ReasonmlPostgresApp.exe bootstrap'

# now that we have build everthing, let's just use the binary

FROM alpine:3.9 as slim

ENV TERM=dumb LD_LIBRARY_PATH=/usr/local/lib:/usr/lib:/lib

WORKDIR /

COPY --from=esy /app/bootstrap ./bootstrap

RUN apk add --no-cache ca-certificates wget bash curl perl-utils git patch gcc g++ musl-dev make m4 gmp-dev linux-headers openssl-dev postgresql-libs postgresql-dev

RUN wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub
RUN wget https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.28-r0/glibc-2.28-r0.apk
RUN apk add --no-cache glibc-2.28-r0.apk

ENTRYPOINT [ "./bootstrap" ]
