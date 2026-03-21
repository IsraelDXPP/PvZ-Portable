import sys
import os
import urllib.request
import http.cookiejar
import re

def download_file_from_google_drive(file_id, destination):
    url = "https://docs.google.com/uc?export=download"
    
    # Setup cookie handling
    cookie_jar = http.cookiejar.CookieJar()
    opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(cookie_jar))
    urllib.request.install_opener(opener)
    
    # First request to get the confirmation token if it's a large file
    req = urllib.request.Request(f"{url}&id={file_id}")
    try:
        print(f"Contacting Google Drive for file ID: {file_id}...")
        response = opener.open(req)
        
        # Check if we got the file directly or a confirmation page
        confirm_token = None
        
        # Check cookies for download_warning
        for cookie in cookie_jar:
            if 'download_warning' in cookie.name:
                confirm_token = cookie.value
                break
        
        if not confirm_token:
            # Fallback: check the page content for "confirm=XXXX"
            content = response.read().decode('utf-8', errors='ignore')
            match = re.search(r'confirm=([0-9A-Za-z_]+)', content)
            if match:
                confirm_token = match.group(1)
        
        if confirm_token:
            print(f"Bypassing large file warning (token found).")
            final_url = f"{url}&id={file_id}&confirm={confirm_token}"
        else:
            final_url = f"{url}&id={file_id}"
            
        print(f"Downloading to {destination}...")
        
        # Download the file
        urllib.request.urlretrieve(final_url, destination)
        print("Download complete.")
        return True
        
    except Exception as e:
        print(f"Error downloading file: {e}")
        return False

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python download_main_pak.py <destination_path>")
        sys.exit(1)
        
    file_id = '1NsQJScVwQSrDI49G2CURiGr67vAS6cuv'
    dest = sys.argv[1]
    
    # Ensure directory exists
    dest_dir = os.path.dirname(os.path.abspath(dest))
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir, exist_ok=True)
    
    success = download_file_from_google_drive(file_id, dest)
    if not success:
        sys.exit(1)
