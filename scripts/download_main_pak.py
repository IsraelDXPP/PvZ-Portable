import sys
import os
import urllib.request
import http.cookiejar
import re
import ssl

def download_file_from_google_drive(file_id, destination):
    url = "https://docs.google.com/uc?export=download"
    
    # Setup cookie handling
    cookie_jar = http.cookiejar.CookieJar()
    
    # Create an SSL context that ignores certificate verification if it fails
    # This helps in environments with broken CA certificates
    try:
        ctx = ssl.create_default_context()
        ctx.check_hostname = False
        ctx.verify_mode = ssl.CERT_NONE
        https_handler = urllib.request.HTTPSHandler(context=ctx)
    except Exception:
        # Fallback for very old python or restricted environments
        https_handler = urllib.request.HTTPSHandler()
    
    opener = urllib.request.build_opener(
        urllib.request.HTTPCookieProcessor(cookie_jar),
        https_handler
    )
    urllib.request.install_opener(opener)
    
    try:
        print(f"Contacting Google Drive for file ID: {file_id}...")
        req = urllib.request.Request(f"{url}&id={file_id}", headers={'User-Agent': 'Mozilla/5.0'})
        response = opener.open(req)
        
        confirm_token = None
        for cookie in cookie_jar:
            if 'download_warning' in cookie.name:
                confirm_token = cookie.value
                break
        
        if not confirm_token:
            content = response.read().decode('utf-8', errors='ignore')
            match = re.search(r'confirm=([0-9A-Za-z_]+)', content)
            if match:
                confirm_token = match.group(1)
        
        if confirm_token:
            print(f"Bypassing large file warning.")
            final_url = f"{url}&id={file_id}&confirm={confirm_token}"
        else:
            final_url = f"{url}&id={file_id}"
            
        print(f"Downloading to {destination}...")
        
        # Download with progress feedback
        def report_progress(block_num, block_size, total_size):
            if total_size > 0:
                percent = min(100, int(block_num * block_size * 100 / total_size))
                if block_num % 200 == 0: # Reduce log noise
                    print(f"Progress: {percent}% ({block_num * block_size // (1024*1024)}MB / {total_size // (1024*1024)}MB)", flush=True)
            elif block_num % 200 == 0:
                 print(f"Progress: {block_num * block_size // (1024*1024)}MB downloaded...", flush=True)

        urllib.request.urlretrieve(final_url, destination, reporthook=report_progress)
        
        if os.path.exists(destination) and os.path.getsize(destination) > 1000000:
            print("Download complete and verified.")
            return True
        else:
            print("Download failed: File empty or too small.")
            return False
        
    except Exception as e:
        print(f"Error during download: {e}", file=sys.stderr)
        return False

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python download_main_pak.py <destination_path>")
        sys.exit(1)
        
    file_id = '1NsQJScVwQSrDI49G2CURiGr67vAS6cuv'
    dest = sys.argv[1]
    
    # Ensure directory exists
    dest_dir = os.path.dirname(os.path.abspath(dest))
    if dest_dir and not os.path.exists(dest_dir):
        os.makedirs(dest_dir, exist_ok=True)
    
    success = download_file_from_google_drive(file_id, dest)
    if not success:
        sys.exit(1)
