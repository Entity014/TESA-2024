from pymongo import MongoClient
import gridfs
from bson.objectid import ObjectId

# MongoDB connection setup
client = MongoClient(
    "mongodb://password:username@192.168.163.251:27017/"
)  # Replace with your connection details
db = client["mydb"]  # Replace with your database name
fs = gridfs.GridFS(db, collection="matlabmodel")  # Replace with your collection name


def download_file(file_name=None, file_id=None, output_path="downloaded_file.onnx"):
    try:
        # Retrieve the file by either file name or file ID
        if file_id:
            file = fs.get(ObjectId(file_id))
        elif file_name:
            file = fs.find_one({"filename": file_name})
            if not file:
                print(f"File with name '{file_name}' not found.")
                return
        else:
            print("Provide either a file_name or file_id.")
            return

        # Write the file content to a local file
        with open(output_path, "wb") as f:
            f.write(file.read())

        print(f"File downloaded successfully as '{output_path}'")
    except Exception as e:
        print(f"Error downloading the file: {e}")


# Example usage
download_file(
    file_name="net.onnx", output_path="net_downloaded.onnx"
)  # Or use file_id if you have it
