FROM python:3
 WORKDIR /socket-back

 COPY requirement.txt .

 COPY subscribe.py .

 RUN pip install --no-cache-dir --upgrade pip
 RUN pip install --no-cache-dir -r requirement.txt
 RUN pip install --no-cache-dir paho-mqtt

 CMD ["python", "mongotoDB.py"]

EXPOSE 7000 
# EXPOSE 6000 for subscribe
# EXPOSE 4000 for publisher

#docker run -dp 0.0.0.0:6000:5000 --name subscribe --restart always subscribe_image
#docker build -t img .