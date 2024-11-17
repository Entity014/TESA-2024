from pymongo import MongoClient
import gridfs
from bson.objectid import ObjectId

# MongoDB setup
client = MongoClient("mongodb://password:username@localhost:27017/")
db = client["mydb"]
fs = gridfs.GridFS(db, collection="matlabmodel")

def download_file(file_name=None, file_id=None, output_path="downloaded_file.onnx"):
    try:
        if file_id:
            # Retrieve the file by ObjectId (file_id)
            file = fs.get(ObjectId(file_id))
        elif file_name:
            # Retrieve the file by filename
            file = fs.find_one({"filename": file_name})
            if not file:
                print(f"File with name '{file_name}' not found.")
                return
            # Print the file_id associated with the file_name
            print(f"File ID for '{file_name}': {file._id}")
        else:
            print("Provide either a file_name or file_id.")
            return

        # Save the file to disk
        with open(output_path, "wb") as f:
            f.write(file.read())
        
        print(f"File downloaded successfully as '{output_path}'")
    except Exception as e:
        print(f"Error downloading the file: {e}")

# Example usage: Print the file_id based on file_name and download the file
download_file(file_name="net.onnx", output_path="net_downloaded.onnx")
