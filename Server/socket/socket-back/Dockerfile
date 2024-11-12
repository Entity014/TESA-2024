FROM python:3
 WORKDIR /socket-back

 COPY requirement.txt .

 COPY publisher.py .

 RUN pip install --no-cache-dir --upgrade pip && \
     pip install --no-cache-dir -r requirement.txt && \
     pip install paho-mqtt
 CMD ["python", "publisher.py"]

EXPOSE 4000
# EXPOSE 5000 for subscribe

 #docker run -dp 0.0.0.0:5000:5000 --name api --restart always api_image
