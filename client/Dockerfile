FROM node:alpine

WORKDIR /talkie-smalkie-client

COPY package*.json ./

RUN npm install

COPY . .

CMD ["npm", "run", "build"]