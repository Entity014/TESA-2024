FROM python:3
WORKDIR /Server/socket/socket-back
COPY app.py .
COPY requirement.txt .
COPY api.py .

RUN pip install --no-cache-dir --upgrade pip && \
    pip install --no-cache-dir -r requirement.txt

CMD ["python", "hello.py"]

EXPOSE 5000