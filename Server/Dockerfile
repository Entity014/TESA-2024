#  DOCKERFILE FOR REACT FRONT END
FROM node:latest
WORKDIR /app
COPY package*.json ./
RUN npm install
COPY . .
RUN npm run build --production
RUN npm install -g serve
EXPOSE 3000
CMD serve -s build